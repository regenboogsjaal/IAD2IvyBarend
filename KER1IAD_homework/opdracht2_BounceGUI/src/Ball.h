
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

	void setup(float _x, float _y, int myRadius);

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

