#include "servoboard.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include "actionfield.h"

class Roboarm {



public:
	Roboarm();
	virtual ~Roboarm();

	void reset(float right_x, float right_y, float right_z, float left_x, float left_y, float left_z);	
	void move(float x, float y, float z);
	void grab(float x, float y, float z);
	void update();

private:
	float map2Coordinates(float x, float y, float z);
	float map2Grabbing(float y);
	float map2Wrist(float z);
	Actionfield a;
	Servoboard& b;
	float maxRightX;
	float maxLeftY;
	float maxLeftZ;
};

