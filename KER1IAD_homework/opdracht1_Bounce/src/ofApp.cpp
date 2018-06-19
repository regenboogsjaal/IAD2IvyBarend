#include "ofApp.h"


void ofApp::setup(){

	last = ofGetElapsedTimeMillis();
	col.setHsb(0, 255, 255);

	for (int i = 0; i < NBALLS; i++) {
		int size = ofRandom(0, 20);
		int randomX = ofRandom(0, ofGetWidth());
		int randomY = ofRandom(0, ofGetHeight());
		myBall[i].setup(randomX, randomY, size);
	}

}


void ofApp::update(){
	ofBackground(col);
	if (ofGetElapsedTimeMillis() - last > 50) {
		col.setHue(counter % 256);
		counter++;
		last = ofGetElapsedTimeMillis();
	}
	for (int i = 0; i < NBALLS; i++) {
		myBall[i].update();
	}
}


void ofApp::draw(){
	for (int i = 0; i < NBALLS; i++) {
		myBall[i].draw();
	}

}

