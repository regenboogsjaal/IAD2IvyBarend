#include<Wire.h>
#include <math.h>
const int MPU=0x68;
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;
double pitch,roll;

int readCount = 0;
int highValCount = 0;
int lastAcX = 0; 
int lastAcY = 0;
int lastAcZ = 0;
int GstillX, GstillY, GstillZ;
int dancingBM = 3000;

double lastPitch = 0;
double curPitch = 0;
double difference;

int waterLevel = 100;

bool dancing = false;
bool dehydrated = false;

const int redPin = 7;
const int greenPin = 6;
const int bluePin = 5;

int r = 0;
int g = 255;
int b = 0;

unsigned long interval = 700;
unsigned long prevMillis = 0;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);

  int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;

  //Acceleration data correction
  AcXoff = -950;
  AcYoff = -300;
  AcZoff = 0;

  //Gyro correction
  GyXoff = 480;
  GyYoff = 170;
  GyZoff = 210;
  
  //read Acc data
  AcX=(Wire.read()<<8|Wire.read()) + AcXoff;
  AcY=(Wire.read()<<8|Wire.read()) + AcYoff;
  AcZ=(Wire.read()<<8|Wire.read()) + AcYoff;
  

  //read gyro data
  GyX=(Wire.read()<<8|Wire.read()) + GyXoff;
  GyY=(Wire.read()<<8|Wire.read()) + GyYoff;
  GyZ=(Wire.read()<<8|Wire.read()) + GyZoff;
  
  readCount++;
  

  //if gyro vals differ startval + dancing benchmark, high val is read
  if (((GyX > GstillX + dancingBM || GyX < GstillX - dancingBM) && 
       (GyY > GstillY + dancingBM || GyY < GstillY - dancingBM) &&
       (GyZ > GstillZ + dancingBM || GyZ < GstillZ - dancingBM)) ||
      ((AcX > lastAcX + dancingBM || AcX < lastAcX - dancingBM) &&
       (AcY > lastAcY + dancingBM || AcY < lastAcY - dancingBM) &&
       (AcZ > lastAcZ + dancingBM || AcZ < lastAcZ - dancingBM)))
  {
    highValCount++;
  }
  //when 30 readings are done;
  if (readCount > 30)
  { //if more than half the readings are high, dancing is true
    if (highValCount > 0.5 * readCount)
    {
      dancing = true;
    } else {
      dancing = false;
    }//reset the reading vals
    highValCount = 0;
    readCount = 0;
  }
  
  Serial.print("dancing bool: ");
  Serial.print(dancing);
  Serial.print("  |  dehydrated bool: ");
  Serial.print(dehydrated);
  Serial.print("  |  readCount: ");
  Serial.print(readCount);
  Serial.print("  |  highValCount: ");
  Serial.print(highValCount);
  Serial.print("  | waterLevel: ");
  Serial.print(waterLevel);
  Serial.print("  | difference: ");
  Serial.println(difference);
  
  getAngle(AcX,AcY,AcZ);
  
  if (dancing)
  {
    if (waterLevel > 0){
    waterLevel--;
    if (waterLevel > 50){
      r = map(waterLevel, 100, 50, 0, 255);
    } else {
      g = map(waterLevel, 0, 50, 0, 255);
    }
  } else {
    dehydrated = true;
    }
  }

  if (dehydrated){
    waterLevel = 0;
    blinkRed();
    if (pitch > lastPitch){
      difference += pitch - lastPitch;
      if (difference > 60){
        waterLevel = 100;
        r = 0;
        g = 255;
        dehydrated = false;
      }
      lastPitch = pitch;
    } else {
      difference = 0;
    }
    lastPitch = pitch;
  }
  
  setColor(r, g, b);
  delay(333);
}

void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void blinkRed(){
  unsigned long curMillis = millis();
  if (curMillis - prevMillis >= interval){
    prevMillis = curMillis;
    if (r == 255){
      r = 0;
    } else {
      r = 255;
    }
  }
}

//convert the accel data to pitch/roll
void getAngle(int Vx,int Vy,int Vz) {
double x = Vx;
double y = Vy;
double z = Vz;

pitch = atan(x/sqrt((y*y) + (z*z)));
roll = atan(y/sqrt((x*x) + (z*z)));
//convert radians into degrees
pitch = pitch * (180.0/3.14);
roll = roll * (180.0/3.14);
}
