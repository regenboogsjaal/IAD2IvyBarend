#pragma once
#include "Ball.h"
#include "ofMain.h"

#define NBALLS 50


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		unsigned long last;
		int counter;
		ofColor col;

		Ball myBall[NBALLS];
		
};
