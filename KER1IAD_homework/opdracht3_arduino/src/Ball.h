
#ifndef Ball_hpp
#define Ball_hpp
#include <stdio.h>
//#endif /*Ball_hpp*/
#pragma once
#include "ofMain.h"


class Ball
{
public:
	Ball();
	~Ball();

	void setup(float _speedX, float _speedY, int _radius, ofColor _color);

	void update();
	void draw();

	float x;
	float y;
	float speedY;
	float speedX;
	int radius;
	ofColor color;

private:

};
#endif

