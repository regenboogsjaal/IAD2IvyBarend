#include "Planet.h"



Planet::Planet()
{
}


//Planet::~Planet()
//{
//}

void Planet::setup(int _speed, float _distance, ofColor _color, int _size) {

	
	
	speed = _speed;
	distance = _distance;
	color = _color;
	size = _size;
	scaledSize = size / 1000;
	radius = scaledSize / 2;
	sunRadius = 150;

	
}

void Planet::update() {

	

	
}

void Planet::draw() {
	
	

	float rot = (0.5 * ofGetFrameNum()) * (speed/4);
	ofPushMatrix();
	ofRotateY(rot);
	ofSetColor(color);
	ofDrawSphere(0, 0, sunRadius + (distance/4), radius);
	ofPopMatrix();
}
