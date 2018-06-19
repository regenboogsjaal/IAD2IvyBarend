#pragma once
#include <stdio.h>
#include "ofMain.h"


class Ball
{
public:
	Ball();
	~Ball();

	void setup(float _x, float _y, float _z, int myRadius);
	void update();
	void draw();

	

	float x;
	float y;
	float z;
	float speedX;
	float speedY;
	float speedZ;

	int radius;

	int boxX;
	int boxY;
	int boxZ;
	ofColor color;
};

