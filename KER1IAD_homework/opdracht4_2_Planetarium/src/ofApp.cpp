#include "ofApp.h"
#define PIN_BUTTON 11

void ofApp::setup(){

	ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);

	arduino.connect("COM4");
	arduino.sendFirmwareVersionRequest();

	ofSetFrameRate(20);
	ofBackground(ofColor::black);

	light.setPointLight();
	light.setDiffuseColor(ofColor::white);
	light.enable();

	startPos.set(600, 0, 600);
	targetPos.set(1500, 1500, 1500);
	startPos = camPos;
	sunRadius = 30;

	for (int i = 0; i < MAXPLANETS; i++) {

		
		planets[0].setup(48, 57, ofColor::silver, 4879); //mercury
		planets[1].setup(35, 108, ofColor::gold, 12104); //venus
		planets[2].setup(30, 150, ofColor::blue, 12756); //earth
		planets[3].setup(24, 228, ofColor::red, 6792); //mars
		planets[4].setup(13, 779, ofColor::orange, 142984); //jupiter
		planets[5].setup(10, 1430, ofColor::beige, 20536); //saturn
		planets[6].setup(7, 2880, ofColor::lightBlue, 51118); //uranus
		planets[7].setup(5, 4500, ofColor::turquoise, 49528); //neptune
	}


}

void ofApp::update(){
	arduino.update();

	
}

void ofApp::draw(){
	ofEnableDepthTest();
	myCam.begin();

	if (drawGrid) ofDrawGrid(200);

	myCam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));

	if (camPos.z < targetPos.z){
		camPos.z += 1;
	}

	if (camPos.x < targetPos.x) {
		camPos.x += 1;
	}

	if (camPos.y < targetPos.y) {
		camPos.y += 1;
	}

	myCam.setGlobalPosition(camPos);
	
	if (sunRadius < 150) {
		sunRadius += 1;
	}

	ofSetColor(ofColor::yellow);
	ofDrawSphere(0, 0, 0, sunRadius);

	

	

	for (int i = 0; i < MAXPLANETS; i++) {
		planets[i].draw();
	}

	myCam.end();
	ofDisableDepthTest();
}




void ofApp::setupArduino(const int& version) {

	ofLogNotice() << "arduino init" << endl;
	ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);
	arduino.sendDigitalPinMode(PIN_BUTTON, ARD_INPUT);
	ofAddListener(arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
}

void ofApp::digitalPinChanged(const int& pin) {
	int value = arduino.getDigital(pin);
	if (pin == PIN_BUTTON && value == 1) {
		drawGrid = !drawGrid;
	}

}

