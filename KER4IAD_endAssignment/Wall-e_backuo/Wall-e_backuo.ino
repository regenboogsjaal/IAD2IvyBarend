#include <Servo.h>
#include <JC_Button.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>


//#define NETWORK_NAME "ei ei ei en ik ben Wi-Fi"
//#define NETWORK_PASSWORD "koffie25"
#define HOST "studenthome.hku.nl"
#define NETWORK_NAME "HKU"
#define NETWORK_PASSWORD "draadloos"

#define THING 1

Button lockButton(2, 25, false, false);
Button confirmButton(10, 25, false, false);
Button cancelButton(4, 25, false, false);
Button location1(11, 25, false, false); //boons
Button location2(12, 25, false, false); //vredenburg markt
Button location3(13, 25, false, false); //vintage island

Servo lockMotor;
int lockOpened = 90;
int lockClosed = 0;

int ldrPin = A0;
int ldrVal = 0;

int lightVal;

const int R = 3;
const int G = 5;
const int B = 6;

String location = "0";
String url = "0";
String response;
int result;

bool thingOpened;
bool budgetApproved;
bool confirmLocation;

String budgetUnit = "bgt: ";
String locationUnit = "loc: ";
String insertLocationUnit = "isl: ";

int budgetIndex;
int locationIndex;
int insertedIndex;

String getBody(const String& response, const String& unit) {
  int bodyStart = response.indexOf(unit);
  int bodyEnd = response.indexOf(F("<br>"), bodyStart + 5);
  String body = response.substring(bodyStart + 5, bodyEnd);
  body.trim();
  return body;
}

void setup() {
  Serial.begin(9600);
  setupESP8266(true); //function from tab //call setupESP8266(true) for forced network connect
  
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  //analogWrite(R, 255); //of (pin, 0); --> common cathode
  //analogWrite(G, 255);
  //analogWrite(B, 255);

  lockButton.begin();
  confirmButton.begin();
  cancelButton.begin();
  location1.begin();
  location2.begin();
  location3.begin();
  
  //setLock(lockClosed);

  location = F("Random");
  Serial.println(location);

  lightVal = 6;
  
  budgetApproved = false;
  confirmLocation = false;
  thingOpened = false;

}

void loop() {
  lockButton.read();
  confirmButton.read();
  cancelButton.read();
  location1.read();
  location2.read();
  location3.read();

  if (location1.wasPressed()){
    location = F("Boons");
    Serial.println(location);
  }
  if (location2.wasPressed()){
    location = F("Episode");
    Serial.println(location);
  }
  if (location3.wasPressed()){
    location = F("MagicMind");
    Serial.println(location);
  }
  

  if (lockButton.wasPressed() && thingOpened == false)
  {
    //send request to retrieve budget
    url = F("/~ivy.barend/KM4_database/balance.php?thingID=");
    url += String(THING);
    result = sendRequest(HOST, url, response);
    if (result < 0) {Serial.println(F("failed to connect to server"));}
    else {
      Serial.println(response);
      budgetIndex = getBody(response, budgetUnit).toInt();
      Serial.println(budgetIndex);
      }

    if (budgetIndex > 0)
    {
      budgetApproved = true;
      Serial.println(F("you've got moneys"));
    } 
    else
    {
      //you're broke
      Serial.println(F("brakka as fakka boi"));
      lights(2);
      delay(200);
      lights(6);
      resetThing();
    }
  }
  
  if (budgetApproved)
  {
    //url = "";
    //check for location
    url = F("/~ivy.barend/KM4_database/location.php?location=");
    url += location;
    url += F("&thingID=");
    url += String(THING);
    result = sendRequest(HOST, url, response);
    if (result < 0) {Serial.println(F("failed to connect to server"));}
    else {
      Serial.println(response);
      locationIndex = getBody(response, locationUnit).toInt();
      }

    if (locationIndex > 0)
    {
      //open the thing
      lights(3);
      delay(200);
      lights(6);
      setLock(lockOpened);
      while (thingOpened == false){
        readLDR();
        if (ldrVal > 500){
          thingOpened = true;
          Serial.println(F("thing opened"));
        }
      }
      budgetApproved = false;
    }
    else if (locationIndex == 0)
    {
      //unusual location
      Serial.println(F("slim shady spotta m8"));
      budgetApproved = false;
      confirmLocation = true;
      
    }
  }

  if (confirmLocation)
  {
    //waiting
    lights(1);
    if (confirmButton.wasPressed())
    {
      //add location to database
      lights(4);
      Serial.println(F("ok fine, adding location to database"));
      url = F("/~ivy.barend/KM4_database/insertLocation.php?location=");
      url += location;
      url += F("&thingID=");
      url += String(THING);
      result = sendRequest(HOST, url, response);
      if (result < 0) {Serial.println(F("failed to connect to server"));}
      else {
        Serial.println(response);
        insertedIndex = getBody(response, insertLocationUnit).toInt();
      }
      if (insertedIndex == 1){
        Serial.println(F("location already exists"));
      }
      if (insertedIndex == 0){
        Serial.println(F("location added"));
      }
      lights(3);
      delay(200);
      lights(6);
      setLock(lockOpened);
      while (thingOpened == false){
        readLDR();
        if (ldrVal > 500){
          thingOpened = true;
        }
      }
      confirmLocation = false;
    }
    if (cancelButton.wasPressed())
    {
      lights(2);
      delay(200);
      lights(6);
      Serial.println(F("the devil salutes you, you resisted your urges"));
      resetThing();
    }
  }

  while (thingOpened == true){
    readLDR();
    if (ldrVal < 500){
      setLock(lockClosed);
      Serial.println(F("thing just closed"));
      thingOpened = false;
      resetThing();
    } else
    {
      Serial.println(F("close the thing you moron"));
    }
  }
  delay(200); 
}

void resetThing()
{
  budgetApproved = false;
  confirmLocation = false;
  lights(5);
  delay(100);
  lights(6);
  
}

void setLock(int angle){
  lockMotor.attach(7);
  int cur_angle1 = lockMotor.read();
  if (cur_angle1 != angle){
    lockMotor.write(angle);
  }
  delay(1000); 
  int cur_angle2 = lockMotor.read();
  if (cur_angle2 = angle){
    lockMotor.detach();
  }
}

void readLDR()
{
  ldrVal = analogRead(ldrPin);
}

void lights(int lightVal){
  switch(lightVal){
    case 1: //blinkblue
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, 255);
    delay(100);
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, 0);
    delay(100);
    break;
    case 2: //red
    analogWrite(R, 255);
    analogWrite(G, 0);
    analogWrite(B, 0);
    break;
    case 3: //green
    analogWrite(R, 0);
    analogWrite(G, 255);
    analogWrite(B, 0);
    break;
    case 4: //magenta
    analogWrite(R, 255);
    analogWrite(G, 51);
    analogWrite(B, 153);
    break;
    case 5: //white
    analogWrite(R, 255);
    analogWrite(G, 255);
    analogWrite(B, 255);
    break;
    case 6:
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, 0);
    break;
  }
}

