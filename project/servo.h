/*
	2009 Benjamin Reh
	ich@benjaminreh.de

	class implementing servos for Mini-Servoboard
	
	!!DO NOT USE THE JUMPER ON THE BOARD!!!
	
*/
#include "serialline.h"
#ifndef SERVO_H
#define SERVO_H
class Servo
{
	public:
	Servo(void);
	~Servo(void);
	
	//initialize by naming it by its number and a outout-connection
	void init(int nr_new, SerialLine* s);

	//Sets/gets speed of a servo. range 0.0 (slowest) ... 1.0 (fastetst)
	float getSpeed(void);
	bool setSpeed(float speed);

	//Sets/gets position of a servo. range 0.0 ... 1.0 
	//Be carefull not every Servo is able to move in the complete Range
	float getPos(void);
	bool setPos(float pos);

	//Sends the new values to the servoboard
	//only values that have been changend are transfered
	bool update();
	
	//Updates in any case, even if the values are unchanged
	//This is usefull, if Servoboard has lost power
	bool force_update();
	
	//removes all information that servos have changed position and speed.
	bool clear_update();

	//Switches the Servo on/off
	void power(bool status);
	bool haspower();
	private:
	float pos;
	float speed;
	bool pos_update;
	bool speed_update;
	bool active;
	int nr;
	SerialLine* s;
};



#endif
