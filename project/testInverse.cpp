#include "Inverse.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv){
	Inverse *inverse=new Inverse();
	
	inverse->setPosition(1,1,1);
	inverse->calcAngles();

	printf("alpha: %f\n",inverse->getAngle(inverse->ANGLE_ALPHA));
	printf("beta:  %f\n",inverse->getAngle(inverse->ANGLE_BETA));
	printf("gamma: %f\n",inverse->getAngle(inverse->ANGLE_GAMMA));
	printf("delta: %f\n",inverse->getAngle(inverse->ANGLE_DELTA));
	
	delete inverse;
	return 0;
}
