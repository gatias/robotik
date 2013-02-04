#include "Inverse.h"
#include "math.h"
#include <stdio.h>

Inverse::Inverse(){
	targetX=targetY=targetZ=0;
}

Inverse::~Inverse(){

}

float Inverse::getAngle(int angle){
	switch(angle){
		case ANGLE_ALPHA: return alpha; break;
		case ANGLE_BETA: return beta; break;
		case ANGLE_GAMMA: return gamma; break;
		case ANGLE_DELTA: return delta; break;
	}
}

void Inverse::setPosition(float x, float y, float z){
	targetX=x;
	targetY=y;
	targetZ=z;
}


void Inverse::setClawPosition(float x, float y, float z){
	targetClawX=x;
	targetClawY=y;
	targetClawZ=z;
}

void Inverse::setRoboarm(Roboarm* arm){
	roboarm=arm;
}

void Inverse::setBounding(int angle, int min, int max){
	switch(angle){
		case ANGLE_ALPHA: 
			alphaMax=max;
			alphaMin=min;
			break;
		case ANGLE_BETA: 
			betaMax=max;
			betaMin=min;
			break;
		case ANGLE_GAMMA: 
			gammaMax=max;
			gammaMin=min;
			break;
		case ANGLE_DELTA: 
			deltaMax=max;
			deltaMin=min;
			break;

	}
}

void Inverse::calcAngles(){
	float l0=roboarm->getBoneLength(BONE_SHOULDER);
	float l1=roboarm->getBoneLength(BONE_HUMERUS);
	float l2=roboarm->getBoneLength(BONE_ELL);

	// Calculate robo arm position	
	alpha=atan2(targetZ, targetX);
	//alpha=atan(targetZ/targetX);
	float tX=sqrt(targetX*targetX+targetZ*targetZ);
	//tX=targetX;
	if(tX>=230)tX=230;
	//printf("tX: %f\n",tX);
	float e=(l1*l1 + tX*tX + targetY*targetY - l2*l2) / (2*l1*sqrt(targetY*targetY + tX*tX));
	float d=(l2*l2 + l1*l1 - tX*tX - targetY*targetY) / (2*l2*l1);
	//beta=acos(e);
//	gamma=acos(d);
	//---fix den state mal so weil des is echt gut grad
	//printf("e=%f acos(e)=%f\n",e,acos(e));
	//printf("d=%f acos(d)=%f\n",d,acos(d));
	//---
	if(e<-1)e=-1;
	if(e>1) e=1;
	if(d<-1)d=-1;
	if(d>1) d=1;
	//printf("e=%f acos(e)=%f\n",e,acos(e));
	//printf("d=%f acos(d)=%f\n",d,acos(d));
	//Der da funktioniert:
//	beta=atan2(tX,targetY) + atan2(e,sqrt(1-e*e));	
	beta=atan(targetY/tX)+acos(e);
	//beta=asin(tX/sqrt(targetY*targetY + tX*tX))-acos(e);
	
	//printf("beta=%f - %f\n",asin(tX/sqrt(targetY*targetY + tX*tX)),acos(e));
	//Die da funktioniert als:
	//gamma=atan2(sqrt(1+d*d),d);

	//Die da sollte richtig sein:
	//gamma=atan2(d,-1*sqrt(1-d*d));
	gamma=acos(d);

}

float Inverse::deg2rad(float deg){
	return deg*PI/180;
}

float Inverse::rad2deg(float rad){
	return rad*(180/PI);
}
void Inverse::setBoneLength(int bone, float length){
	/*switch(bone){
		case BONE_SHOULDER:
			shoulderLength=length;
			break;
		case BONE_HUMERUS:
			humerusLength=length;
			break;
		case BONE_ELL:
			ellLength=length;
			break;
		case BONE_HAND:
			handLength=length;
			break;
	}*/

}
