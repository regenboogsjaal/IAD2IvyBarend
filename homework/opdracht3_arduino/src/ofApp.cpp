#include "ofApp.h"

#define PIN_BUTTON_LEFT 12
#define PIN_BUTTON_RIGHT 13
#define PIN_LDR 0
#define PIN_POT 1
#define PIN_LED 11

void ofApp::setup(){
	gui.setup("instellingen", "settings.xml");
	gui.add(radius.set("Radius", 50, 10, 200));
	gui.add(speedX.set("speedX", 5, 5, 30));
	gui.add(speedY.set("speedY", 5, 5, 30));
	gui.add(color.set("color", ofColor::blue));

	ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);
	arduino.connect("COM4");
	arduino.sendFirmwareVersionRequest();
}


void ofApp::update(){
	arduino.update();
	for (unsigned int i = 0; i < balls.size(); i++) {
		balls[i].update();
	}
}


void ofApp::draw(){
	for (unsigned int i = 0; i < balls.size(); i++) {
		balls[i].draw();
	}

	gui.draw();
}


void ofApp::keyPressed(int key){
	addBall();
}



void ofApp::mousePressed(int x, int y, int button) {
	if (balls.size() > 0) {
		balls.pop_back();
	}
}

void ofApp::addBall() {
	Ball newBall;
	newBall.setup(speedX, speedY, radius, color);
	balls.push_back(newBall);
}

void ofApp::setupArduino(const int& version) {
	//ofLog() << "arduino firmware found" << arduino.getFirmwareName()
	//	<< arduino.getMajorFirmwareVersion()
	//	<< arduino.getMinorFirmwareVersion() << endl;
	ofLogNotice() << "arduino init" << endl;
	ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);

	arduino.sendDigitalPinMode(PIN_LED, ARD_OUTPUT); //ledje, digital
	arduino.sendDigitalPinMode(PIN_BUTTON_LEFT, ARD_INPUT); //knopje links AAN, digital
	arduino.sendDigitalPinMode(PIN_BUTTON_RIGHT, ARD_INPUT); //knopje rechts UIT, digital
	arduino.sendAnalogPinReporting(PIN_LDR, ARD_ANALOG); //lichtsensor, analog
	arduino.sendAnalogPinReporting(PIN_POT, ARD_ANALOG); //podmeter, analog

	ofAddListener(arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
	ofAddListener(arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
}


void ofApp::analogPinChanged(const int& pin) {
	
	int value = arduino.getAnalog(pin);
	ofLogNotice() << "analog pin #" << pin << "changed to value " << value << endl;

	if (pin == 1) {
		radius = ofMap(value, 0, 1024, radius.getMin(), radius.getMax());
	}
	else if (pin == PIN_LDR) {
		float colorHue = ofMap(value, 0, 1024, 0, 255);
		ofColor newColor = ofColor(color.get());
		newColor.setHue(colorHue);
		color = newColor;
	}
}

void ofApp::digitalPinChanged(const int& pin) {
	int value = arduino.getDigital(pin);
	ofLogNotice() << "digital pin #" << pin << "changed to value " << value << endl;

	if (pin == PIN_BUTTON_LEFT && value == 1) { // if button is pressed
		addBall();
		arduino.sendDigital(PIN_LED, 1);
	}
	else {
		arduino.sendDigital(PIN_LED, 0);
	}

	if (pin == PIN_BUTTON_RIGHT && value == 1){
		arduino.sendDigital(PIN_LED, 1);
		if (balls.size() > 0) {
			balls.pop_back();
		}
	}
	else {
		arduino.sendDigital(PIN_LED, 0);
	}

	//while (buttonPressing()) {
	//	arduino.sendDigital(PIN_LED, 1);
	//}

}



//ofLogVerbose 