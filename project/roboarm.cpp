#include "roboarm.h"

using namespace std;


Roboarm::Roboarm():
	a("/dev/ttyS0"),
	b(*(a.getServoboard())),
	maxRightX(0.0f),
	maxLeftY(0.0f),
	maxLeftZ(0.0f)
{

	if(!b.isConnected()) {
		cout << "Verbindung konnte nicht aufgebaut werden ... " << endl;
		exit(-1);
	}
}

Roboarm::~Roboarm() {
	b.disconnect();
}

void Roboarm::reset (float right_x, float right_y, float right_z, float left_x, float left_y, float left_z) {
	maxRightX = right_x;
	maxLeftY = left_y;
	maxLeftZ = left_x;
	if(maxRightX<0)maxRightX*=-1;
	if(maxLeftY<0)maxLeftY*=-1;
	if(maxLeftZ<0)maxLeftZ*=-1;
	a.resetArm();
}

void Roboarm::move(float x, float y, float z) {
	b.servo[2].setPos( map2Coordinates(x, y, z) );
	//update();
}

void Roboarm::grab(float x, float y, float z) {
	b.servo[4].setPos(map2Grabbing(y));
	b.servo[5].setPos(map2Wrist(z));
}
void Roboarm::update(){
	b.updateSimultaneously(100);
}

float Roboarm::map2Coordinates(float x, float y, float z) {
	float pos = 0.5f;
	if(x<0)x*=-1;
	if(x>maxRightX)
		maxRightX=x;
	//if(x < 0) 
	//	x = 0.1f;
	pos = x / maxRightX;

	if(pos < 0.14)
		pos = 0.14;
	if(pos > 0.9)
		pos = 0.9;
	return pos;
}


float Roboarm::map2Grabbing(float y) {
	if(y<0)y*=-1;
	float pos = 0.5f;
//	if(y < 0) 
//		y = 0.5f;
	if(y>maxLeftY)
		maxLeftY=y;
	pos = y / maxLeftY;
	if(pos < 0.5)
		pos = 0.5;
	if(pos > 1)
		pos = 1;

	return pos;
}


float Roboarm::map2Wrist(float z) {
	float pos = 0.5f;
	if(z<0)z*=-1;
	if(z>maxLeftZ)
		maxLeftZ=z;
//	if(z < 0) 
//		z = 0.1f;
	pos = z / maxLeftZ;
	if(pos < 0.1)
		pos = 0.1;
	if(pos > 0.9)
		pos = 0.9;
	return pos;
}
