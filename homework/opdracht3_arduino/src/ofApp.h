#pragma once

#include "ofMain.h"
#include "Ball.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		
		
	private:
		ofArduino arduino;

		void setupArduino(const int& version);

		void analogPinChanged(const int& pin);
		void digitalPinChanged(const int& pin);

		
		vector<Ball> balls;

		void addBall();

		ofxPanel gui;
		ofParameter<int> radius;
		ofParameter<int> speedX;
		ofParameter<int> speedY;
		ofParameter<ofColor> color;
		

};
