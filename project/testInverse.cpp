#include "Inverse.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "roboarm.h"

using namespace std;

int main(int argc, char** argv){
	Inverse *inverse=new Inverse();
	Roboarm *roboarm=new Roboarm();
	
	//Set angle ranges of robot
	//inverse->setBounding(Inverse::ANGLE_ALPHA,inverse->deg2rad(roboarm->getMinDeg(Inverse::ANGLE_ALPHA)),inverse->deg2rad(45)); 
	//inverse->setBounding(Inverse::ANGLE_BETA,inverse->deg2rad(10),inverse->deg2rad(160));
	//inverse->setBounding(Inverse::ANGLE_GAMMA,inverse->deg2rad(10),inverse->deg2rad(170)); 
	//inverse->setBounding(Inverse::ANGLE_DELTA,0,inverse->deg2rad(180)); //foot

	inverse->setRoboarm(roboarm);
	
	//TODO: Arm ausmessen
	//Set lengths of bones of robot
	inverse->setBoneLength(Inverse::BONE_SHOULDER,1);
	inverse->setBoneLength(Inverse::BONE_HUMERUS,1);
	inverse->setBoneLength(Inverse::BONE_ELL,1);
	inverse->setBoneLength(Inverse::BONE_HAND,1);

	//Set new target position
	inverse->setPosition(1,1,1);	
	//Calculate new angles
	inverse->calcAngles();

	//TODO: Send angles to robot

	printf("alpha: %f\n",inverse->getAngle(Inverse::ANGLE_ALPHA));
	printf("beta:  %f\n",inverse->getAngle(Inverse::ANGLE_BETA));
	printf("gamma: %f\n",inverse->getAngle(Inverse::ANGLE_GAMMA));
	printf("delta: %f\n",inverse->getAngle(Inverse::ANGLE_DELTA));
	
	delete inverse;
	return 0;
}
