#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Planet.h"

#define MAXPLANETS 8
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		

		bool drawGrid = false;
		
		ofLight light;
		ofCamera myCam;
		

		Planet planets[MAXPLANETS];

		
		ofVec3f startPos;
		ofVec3f targetPos;
		ofVec3f camPos;

		int sunRadius;

		ofArduino arduino;
		void setupArduino(const int& version);
		void digitalPinChanged(const int& pin);
};
