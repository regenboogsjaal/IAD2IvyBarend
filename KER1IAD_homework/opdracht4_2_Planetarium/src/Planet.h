#pragma once
#include <stdio.h>
#include "ofMain.h"

class Planet
{
	public:
		Planet();
		//~Planet();

		void setup(int _speed, float _distance, ofColor _color, int _size);
		void update();
		void draw();

		float rot;

	
		float distance;

		int speed;
		int size;
		int scaledSize;

		
		ofColor color;

		int radius;
		int sunRadius;

		

};

