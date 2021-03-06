#include "roboarm.h"
#include <stdlib.h>
#include "Inverse.h"
#include <stdio.h>
#include <math.h>

using namespace std;


Roboarm::Roboarm():
	a("/dev/ttyS0"),
	b(*(a.getServoboard()))
{

	if(!b.isConnected()) {
		cout << "Verbindung konnte nicht aufgebaut werden ... " << endl;
		exit(-1);
	}
	// smoothing threshold in degree
	threshold = 5.0; 

	//Time the robo gets for each move
	millisecondsPerMove=500;

	//set maximum and minimum values
	//that can be sent to the robo

	minRob[Inverse::ANGLE_ALPHA]=0;
	maxRob[Inverse::ANGLE_ALPHA]=0.84;

	minRob[Inverse::ANGLE_BETA]=0.3;
	maxRob[Inverse::ANGLE_BETA]=1;

	minRob[Inverse::ANGLE_GAMMA]=0;
	maxRob[Inverse::ANGLE_GAMMA]=0.8;

	minRob[Inverse::ANGLE_DELTA]=0.25;
	maxRob[Inverse::ANGLE_DELTA]=1;

	minRob[Inverse::ANGLE_EPSILON]=0;
	maxRob[Inverse::ANGLE_EPSILON]=1;
	
	minRob[Inverse::ANGLE_ZETA]=0.5;
	maxRob[Inverse::ANGLE_ZETA]=1;
	

	//set maximum and minimum values for the angles in degree

	minDeg[Inverse::ANGLE_ALPHA]=0;
	maxDeg[Inverse::ANGLE_ALPHA]=180;

	minDeg[Inverse::ANGLE_BETA]=45;
	maxDeg[Inverse::ANGLE_BETA]=180;

	minDeg[Inverse::ANGLE_GAMMA]=10;
	maxDeg[Inverse::ANGLE_GAMMA]=160;

	minDeg[Inverse::ANGLE_DELTA]=10;
	maxDeg[Inverse::ANGLE_DELTA]=170;

	minDeg[Inverse::ANGLE_EPSILON]=5;
	maxDeg[Inverse::ANGLE_EPSILON]=180;
	
	minDeg[Inverse::ANGLE_ZETA]=0;
	maxDeg[Inverse::ANGLE_ZETA]=50;
	

	boneLength[Inverse::BONE_SHOULDER]=78;
	boneLength[Inverse::BONE_HUMERUS]=154;
	boneLength[Inverse::BONE_ELL]=185;
	boneLength[Inverse::BONE_HAND]=45;

	multiplayer=false;

}

void Roboarm::setMillisecondsPerMove(int millis){
	millisecondsPerMove=millis;
}

Roboarm::~Roboarm() {
	b.disconnect();
}

float Roboarm::getBoneLength(int bone){
	return boneLength[bone];
}

float Roboarm::getMinDeg(int angle){
	return minDeg[angle];
}
float Roboarm::getMaxDeg(int angle){
	return maxDeg[angle];
}

void Roboarm::reset () {
	/*maxRightX = right_x;
	maxLeftY = left_y;
	maxLeftZ = left_x;
	if(maxRightX<0)maxRightX*=-1;
	if(maxLeftY<0)maxLeftY*=-1;
	if(maxLeftZ<0)maxLeftZ*=-1;*/
	a.resetArm();
}

float Roboarm::deg2rob(int angle, float deg){
	float rob=0;
	//max and min bounding for angles
	if(deg>maxDeg[angle])deg=maxDeg[angle];
	if(deg<minDeg[angle])deg=minDeg[angle];

	if(angle==Inverse::ANGLE_BETA)deg-=45;
	if(angle==Inverse::ANGLE_DELTA)deg+=85;
	if(angle==Inverse::ANGLE_EPSILON)deg-=5;
	rob=deg/(maxDeg[angle]-minDeg[angle])*(maxRob[angle]-minRob[angle]);

	if(angle==Inverse::ANGLE_BETA)rob=1-rob;
	if(angle==Inverse::ANGLE_GAMMA)rob=1-rob;
	if(angle==Inverse::ANGLE_ZETA)rob=1-rob;
	if(angle==Inverse::ANGLE_EPSILON)rob=1-rob;
	return rob;
}


bool Roboarm::move(float x, float y, float z) {
	z=z*-1;
	//if(x<1) x=1;
	//if(y<1) y=1;
	//if(z<1) z=1;

	/*if(x>maxRightX)
		maxRightX=x;
	if(y>maxRightY)
		maxRightY=y;
	if(z>maxRightZ)
		maxRightZ=z;
	*/
	float max=230;
	float ymax=400;



	Inverse *inverse=new Inverse();
	inverse->setRoboarm(this);

	//Verschieben des Koordinatenursprungs und Skalierung.
	z=(z-200)/2;
	y=(y+200)/2;
	x=(x-200)/2;
	
	if(x>max)x=max;
	if(x<-1*max)x=-1*max;
	if(y>ymax)y=ymax;
	if(z>max)z=max;
	if(z<-1*max)z=-1*max;
	inverse->setPosition(x,y,z);	

	// Calculate all Angles
	inverse->calcAngles();
	//currentAngle[Inverse::ANGLE_ALPHA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_ALPHA));
	//currentAngle[Inverse::ANGLE_BETA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_BETA));
	//currentAngle[Inverse::ANGLE_GAMMA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_GAMMA));
	
	// smoothing

	//used to indicate that some angles changed
	bool moved=false;

	float newAngle = 0.0;
	for(int i = 0; i < 3; i++) {
		newAngle =  inverse->rad2deg(inverse->getAngle(i));
		if(abs(currentAngle[i] - newAngle) > threshold) {
			currentAngle[i] = newAngle;
			moved=true;
		}
	}

	b.servo[0].setPos(deg2rob(Inverse::ANGLE_ALPHA,currentAngle[Inverse::ANGLE_ALPHA]));
	b.servo[1].setPos(deg2rob(Inverse::ANGLE_BETA,currentAngle[Inverse::ANGLE_BETA]));
	b.servo[2].setPos(deg2rob(Inverse::ANGLE_GAMMA,currentAngle[Inverse::ANGLE_GAMMA]));
	delete inverse;
	
	return moved;
}
void Roboarm::setMultiplayer(bool m){
	multiplayer=m;
}
bool Roboarm::isMultiplayer(){
	return multiplayer;
}

bool Roboarm::grab(float lx, float ly, float rx, float ry) {
	Inverse *inverse=new Inverse();
	inverse->setAngle(Inverse::ANGLE_ALPHA,currentAngle[Inverse::ANGLE_ALPHA]);
	inverse->setAngle(Inverse::ANGLE_BETA,currentAngle[Inverse::ANGLE_BETA]);
	inverse->setAngle(Inverse::ANGLE_GAMMA,currentAngle[Inverse::ANGLE_GAMMA]);
	inverse->setRoboarm(this);

	ly=ly+200;
	ry=ry+200;
	inverse->setClawPosition(lx,ly,rx,ry);
	inverse->calcClawAngles();
	currentAngle[Inverse::ANGLE_DELTA] = inverse->getAngle(Inverse::ANGLE_DELTA);
	b.servo[3].setPos(deg2rob(Inverse::ANGLE_DELTA,currentAngle[Inverse::ANGLE_DELTA]));
	
	currentAngle[Inverse::ANGLE_ZETA] = inverse->getAngle(Inverse::ANGLE_ZETA);
	//printf("zeta out: %f\n", deg2rob(Inverse::ANGLE_ZETA,currentAngle[Inverse::ANGLE_ZETA])) ;
	b.servo[4].setPos(deg2rob(Inverse::ANGLE_ZETA,currentAngle[Inverse::ANGLE_ZETA]));
	//b.servo[4].setPos(0.8);


	if(multiplayer){
		// rotate grabber
		if(rx <=0.0)
			rx = 0.1;

		float height = (ly - ry)/2;
		//Das hier ist noch um 90° verdreht, braucht noch kurz Hirnschmalz
		currentAngle[Inverse::ANGLE_EPSILON] = 180-inverse->rad2deg(atan2(rx,height));
		b.servo[5].setPos(deg2rob(Inverse::ANGLE_EPSILON,currentAngle[Inverse::ANGLE_EPSILON]));
	}
		//printf("gamma out: %f\n", currentAngle[Inverse::ANGLE_GAMMA]);


	delete inverse;

	return true;
}
void Roboarm::update(){
	b.updateSimultaneously(millisecondsPerMove);
}

float Roboarm::map2Coordinates(float x, float y, float z) {
	float pos = 0.5f;
	if(x<0)x*=-1;
/*	if(x>maxRightX)
		maxRightX=x;
	if(x>maxRightX)
		maxRightX=x;
	if(x>maxRightX)
		maxRightX=x;*/
	//if(x < 0) 
	//	x = 0.1f;
	//pos = x / maxRightX;

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
