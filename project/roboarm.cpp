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
	threshold = 2.0; 

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

	minRob[Inverse::ANGLE_DELTA]=0;
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
	if(angle==Inverse::ANGLE_DELTA)deg-=10;
	rob=deg/(maxDeg[angle]-minDeg[angle])*(maxRob[angle]-minRob[angle]);

	if(angle==Inverse::ANGLE_BETA)rob=1-rob;
	if(angle==Inverse::ANGLE_DELTA)rob=1-rob;
	return rob;
}



void Roboarm::move(float x, float y, float z) {
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
	float max=boneLength[Inverse::BONE_HUMERUS]+boneLength[Inverse::BONE_ELL];



	Inverse *inverse=new Inverse();
	inverse->setRoboarm(this);

	//Verschieben des Koordinatenursprungs und Skalierung.
	z=(z-200)/2;
	y=(y+200)/2;
	x=(x-200)/2;
	
	if(x>max)x=max;
	if(x<-1*max)x=-1*max;
	if(y>max)y=max;
	if(z>max)z=max;
	if(z<-1*max)z=-1*max;
//	inverse->setPosition(-339,208,25);	
	//inverse->setPosition(200,100,100);	
	inverse->setPosition(x,y,z);	

	// Calculate all Angles
	inverse->calcAngles();
	currentAngle[Inverse::ANGLE_ALPHA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_ALPHA));
	currentAngle[Inverse::ANGLE_BETA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_BETA));
	currentAngle[Inverse::ANGLE_GAMMA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_GAMMA));
//	currentAngle[Inverse::ANGLE_DELTA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_DELTA));
//	currentAngle[Inverse::ANGLE_EPSILON] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_EPSILON));
//	currentAngle[Inverse::ANGLE_ZETA] = inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_ZETA));
	

	// smoothing
	float newAngle = 0.0;
	for(int i = 0; i < 3; i++)
	{
		newAngle =  inverse->rad2deg(inverse->getAngle(i));
		if(abs(currentAngle[i] - newAngle) > threshold)
		{
			currentAngle[i] = newAngle;
		}
	}

	b.servo[0].setPos(deg2rob(Inverse::ANGLE_ALPHA,currentAngle[Inverse::ANGLE_ALPHA]));
	//printf("alpha: %f\n",inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_ALPHA)));
	//printf("beta:  %f\n",inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_BETA)));
	//printf("gamma: %f\n",inverse->rad2deg(inverse->getAngle(Inverse::ANGLE_GAMMA)));
	//printf("x: %f y: %f z:%f\n", x,y,z);
	//printf("---\n");
	b.servo[1].setPos(deg2rob(Inverse::ANGLE_BETA,currentAngle[Inverse::ANGLE_BETA]));
	b.servo[2].setPos(deg2rob(Inverse::ANGLE_GAMMA,currentAngle[Inverse::ANGLE_GAMMA]));
	//b.servo[0].setPos(deg2rob(Inverse::ANGLE_ALPHA,90));
	//deg2rob()
	//b.servo[1].setPos(deg2rob(Inverse::ANGLE_BETA,180));
	//b.servo[2].setPos(deg2rob(Inverse::ANGLE_GAMMA,90));
	//printf("0: %f\n",b.servo[0].getPos());
	//printf("1: %f\n",b.servo[1].getPos());
	///printf("2: %f\n",b.servo[2].getPos());
	delete inverse;

	//b.servo[2].setPos( map2Coordinates(x, y, z) );
	//update();
}

void Roboarm::grab(float lx, float ly, float rx, float ry) {
	Inverse *inverse=new Inverse();
	inverse->setRoboarm(this);

	ly=ly+200;
	ry=ry+200;
	inverse->setClawPosition(lx,ly,rx,ry);
	inverse->calcClawAngles();
	currentAngle[Inverse::ANGLE_DELTA] = inverse->getAngle(Inverse::ANGLE_DELTA);
	b.servo[3].setPos(deg2rob(Inverse::ANGLE_DELTA,currentAngle[Inverse::ANGLE_DELTA]));
	
	currentAngle[Inverse::ANGLE_ZETA] = inverse->getAngle(Inverse::ANGLE_ZETA);
	printf("zetaDeg: %f\n", deg2rob(Inverse::ANGLE_ZETA,currentAngle[Inverse::ANGLE_ZETA])) ;
	b.servo[4].setPos(deg2rob(Inverse::ANGLE_ZETA,currentAngle[Inverse::ANGLE_ZETA]));
	delete inverse;
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
