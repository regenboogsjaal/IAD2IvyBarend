#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup("Instellingen", "settings.xml");
	gui.add(radius.set("Radius", 50, 0, 200));
	gui.add(speedX.set("Speed X", 20, 0, 200));
	gui.add(speedY.set("Speed Y", 20, 0, 200));
	gui.add(color.set("Color", ofColor::white));
} 

void ofApp::update(){
	for (unsigned int i = 0; i < balls.size(); i++) {
		balls[i].update();
	}

}


void ofApp::draw(){
	gui.draw();
	for (unsigned int i = 0; i < balls.size(); i++) {
		balls[i].draw();
	}
}

void ofApp::keyPressed(int key){
	Ball newBall;
	newBall.setup(speedX, speedY, radius);
	balls.push_back(newBall);
	

}

void ofApp::mousePressed(int x, int y, int button){
	if (balls.size() > 0) {
		balls.pop_back();
	}
}

