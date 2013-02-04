#ifndef ROBOARM_H
#define ROBOARM_H

#include "servoboard.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include "actionfield.h"

class Roboarm {



public:
	Roboarm();
	virtual ~Roboarm();

	void reset();	
	void move(float x, float y, float z);
	void grab(float xl, float yl, float xr, float yr);
	void update();
	float deg2rob(int angle, float deg);
	float getMinDeg(int angle);
	float getMaxDeg(int angle);
	float getBoneLength(int bone);
	void setMillisecondsPerMove(int millis);
private:
	float map2Coordinates(float x, float y, float z);
	float map2Grabbing(float y);
	float map2Wrist(float z);
	Actionfield a;
	Servoboard& b;
	float maxLeftY;
	float maxLeftZ;
	float minRob[4];
	float maxRob[4];
	float minDeg[4];
	float maxDeg[4];
	float boneLength[4];
	float currentAngle[4];
	float threshold;
	int millisecondsPerMove;
};
#endif
