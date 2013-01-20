#include "Inverse.h"
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
	alpha=beta=gamma=delta=42;

}

float Inverse::deg2rad(float deg){
	return deg*PI/180;
}

void Inverse::setBoneLength(int bone, float length){
	switch(bone){
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
	}

}
