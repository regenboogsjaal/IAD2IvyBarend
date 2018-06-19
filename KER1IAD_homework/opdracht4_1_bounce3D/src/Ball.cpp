#include "Ball.h"



Ball::Ball()
{
}


Ball::~Ball()
{
}


void Ball::setup(float _x, float _y, float _z, int myRadius) {
	x = _x;
	y = _y;
	z = _z;

	radius = myRadius;

	boxX = 1000;
	boxY = 1000;
	boxZ = 1000;

	speedX = ofRandom(-5, 5);
	speedY = ofRandom(-5, 5);
	speedZ = ofRandom(-5, 5);

	color.set(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));

	

}

void Ball::update() {
	if (x < radius) {
		x = radius;
		speedX *= -1;
	}
	else if (x > boxX - (radius)) {
		x = boxX - (radius);
		speedX += -1;
	}

	if (y < radius) {
		y = radius;
		speedY *= -1;
	}
	else if (y > boxY - (radius)) {
		y = boxY - (radius);
		speedY *= -1;
	}

	if (z < radius) {
		z = radius;
		speedZ *= -1;
	}
	else if (z > boxZ - (radius)) {
		z = boxZ - (radius);
		speedZ *= -1;
	}

	x += speedX;
	y += speedY;
	z += speedZ;
}

void Ball::draw() {
	ofSetColor(color);
	ofDrawSphere(x, y, z, radius);
	
}