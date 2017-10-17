#include "ofApp.h"


void ofApp::setup(){
	ofBackground(ofColor::black);

	gui.setup("Instellingen", "settings.xml");
	gui.add(radius.set("Radius", 50, 0, 200));
	gui.add(speedX.set("Speed X", 20, 0, 200));
	gui.add(speedY.set("Speed Y", 20, 0, 200));
	gui.add(speedZ.set("Speed Z", 20, 0, 200));

	gui.add(drawGrid.set("GRID", false));
	gui.add(useLight.set("LIGHT", false));

	gui.add(color.set("Color", ofColor::white));

	light.setDirectional();
	light.setDiffuseColor(ofColor::white);
	light.enable();

	cam.setPosition(ofVec3f(500, 500, -1500));
	cam.lookAt(ofVec3f(500, 500, 500));

	light.setPosition(500, 500, -1500);
	light.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
}

void ofApp::update(){
	for (unsigned int i = 0; i < balls.size(); i++) {
		balls[i].update();
	}
}

void ofApp::draw(){
	
	ofEnableDepthTest();
	//cam.setVFlip(true);
	cam.begin();
	
	if (drawGrid) ofDrawGrid(100);

	if (useLight) {
		ofEnableLighting();
	}
	else {
		ofDisableLighting();
	}

	for (unsigned int i = 0; i < balls.size(); i++) {
		balls[i].draw();
	}

	ofSetColor(ofColor::white, 100);
	//ofSetLineWidth(0.4f);
	ofDrawBox(ofPoint(500, 500, 500), 1000);

	cam.end();

	ofDisableDepthTest();

	if (drawGui) {
		ofDisableLighting();
		gui.draw();
	}
}


void ofApp::keyPressed(int key){
	

	if (key == ' ') {
		drawGui = !drawGui;
	}
	if (key == 'l') {
		useLight = !useLight;
	}
	if (key == 'g') {
		drawGrid = !drawGrid;
	}

	if (key == 'b'){
		Ball newBall;
		newBall.setup(speedX, speedY, speedZ, radius);
		balls.push_back(newBall);
	}

	if (key == 'd') {
		if (balls.size() > 0) {
			balls.pop_back();
		}
	}
}



