#include "Ball.h"



Ball::Ball()
{
}

void Ball::setup(float _x, float _y, int myRadius) {
	x = _x;
	y = _y;
	radius = myRadius;

	speedX = ofRandom(-5, 5);
	speedY = ofRandom(-5, 5);

	color.set(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
}

void Ball::update() {
	if (x < radius) {
		x = radius;
		speedX *= -1;
	}
	else if (x > ofGetWidth()-(radius)) {
		x = ofGetWidth()-(radius);
		speedX += -1;
	}

	if (y < radius) {
		y = radius;
		speedY *= -1;
	}
	else if (y > ofGetHeight()-(radius)) {
		y = ofGetHeight()-(radius);
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
