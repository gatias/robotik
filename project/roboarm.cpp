#include "roboarm.h"
#include <stdlib.h>
#include "Inverse.h"

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
	//set maximum and minimum values
	//that can be sent to the robo
	minRob[Inverse::ANGLE_ALPHA]=0.3;
	maxRob[Inverse::ANGLE_ALPHA]=1;

	minRob[Inverse::ANGLE_BETA]=0;
	maxRob[Inverse::ANGLE_BETA]=0.8;

	minRob[Inverse::ANGLE_GAMMA]=0;
	maxRob[Inverse::ANGLE_GAMMA]=1;

	minRob[Inverse::ANGLE_DELTA]=0;
	maxRob[Inverse::ANGLE_DELTA]=0.84;

	//set maximum and minimum values for the angles in degree
	minDeg[Inverse::ANGLE_ALPHA]=-90;
	maxDeg[Inverse::ANGLE_ALPHA]=45;

	minDeg[Inverse::ANGLE_BETA]=10;
	maxDeg[Inverse::ANGLE_BETA]=160;

	minDeg[Inverse::ANGLE_GAMMA]=10;
	maxDeg[Inverse::ANGLE_GAMMA]=170;

	minDeg[Inverse::ANGLE_DELTA]=0;
	maxDeg[Inverse::ANGLE_DELTA]=180;
}

Roboarm::~Roboarm() {
	b.disconnect();
}

float Roboarm::getMinDeg(int angle){
	return minDeg[angle];
}
float Roboarm::getMaxDeg(int angle){
	return maxDeg[angle];
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

float Roboarm::deg2rob(int angle, float deg){
	float rob=0;
	
	return rob;
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
