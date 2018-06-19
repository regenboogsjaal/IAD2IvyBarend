#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Ball.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		
		

		vector<Ball> balls;

		ofxPanel gui;
		ofParameter<int> radius;
		ofParameter<float> speedX;
		ofParameter<float> speedY;
		ofParameter<float> speedZ;
		ofParameter<ofColor> color;

		ofParameter<bool> drawGrid;
		ofParameter<bool> useLight;
		bool drawGui = true;

		ofLight light;
		ofCamera cam;
		//ofEasyCam cam;
		
		
		
};
