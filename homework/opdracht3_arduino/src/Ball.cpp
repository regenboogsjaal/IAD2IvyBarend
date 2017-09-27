#include "Ball.h"

Ball::Ball()
{
}

void Ball::setup(float _speedX, float _speedY, int _radius, ofColor _color) {
	x = ofRandom(0, ofGetWidth());
	y = ofRandom(0, ofGetHeight());
	
	radius = _radius;

	speedX = _speedX;
	speedY = _speedX;

	color = _color;
}

void Ball::update() {
	if (x < radius) {
		x = radius;
		speedX *= -1;
	}
	else if (x > ofGetWidth() - (radius)) {
		x = ofGetWidth() - (radius);
		speedX *= -1;
	}

	if (y < radius) {
		y = radius;
		speedY *= -1;
	}
	else if (y > ofGetHeight() - (radius)) {
		y = ofGetHeight() - (radius);
		speedY *= -1;
	}

	x += speedX;
	y += speedY;
}

void Ball::draw() {
	ofSetColor(color);
	ofDrawCircle(x, y, radius);
}

Ball::~Ball()
{
}
