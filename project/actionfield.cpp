/**
* July 2009 Edward-Robert Tyercha
* March 2012 Edward-Robert Tyercha (Update for silver-arm)
**/


#include "actionfield.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include "servoboard.h"

using namespace std;

Actionfield::Actionfield(char* dev) 
{
	this->init(dev);
}

Actionfield::Actionfield() 
{
    this->init("/dev/ttyS0");
}


void Actionfield::init(char* dev)
{
    mFieldWidth = 20.0; // Breite und Höhe vom Feld
    mFieldHeight = 20.0;
    mOffsetS0X = 10.0; // Position von S0 relativ zum Ursprung
    mOffsetS0Y = -10.0;
    mOffsetS0Z =  0.0;
    
    time_to_move = 1000;
	time_to_wait = static_cast<float> (time_to_move);
	
	mBoard = new Servoboard(6);
    if (!(mBoard->connect(dev)))
    {
        cerr << "Error connecting to "<< dev << endl;
    }
	
}

Actionfield::~Actionfield()
{
    delete mBoard;
}

float covertRadianToServoValues(float Rl, float Rr,
                               float Sl, float Sr,
                               float r )  {
	 return (r-Rl) * (Sr-Sl)/(Rr-Rl) + Sl;
}

float Actionfield::getS0ServoValue(const float O_P_x, const float O_P_y, const float O_P_z)
{
    /*
    * translate coordinates to rotation center of servo0
    */
    float S0_P_x = O_P_x - this->mOffsetS0X;
    float S0_P_y = O_P_y - this->mOffsetS0Y;
    //float S0_P_z = O_P_z - this->mOffsetS0Z;


    /*
    * calculate distance from rotation center Servo0  to (x,y)
    */
    float len_S0_P = sqrt( S0_P_x*S0_P_x + S0_P_y*S0_P_y );

    /*
    * the arm has a x-offset value, thus we need to rotate
    * servo0 a little bit more.
    */
    float radian_offset = asin( OFFSET_SERVO1_X / len_S0_P );

    /*
    * this value calculates the radian value to (x,y) with an
    * imaginary arm merged at the center of the rotation axis S0
    */
    float radian_arm = acos( S0_P_x / len_S0_P );

    /*
    * calculate the real radian value to rotate servo0
    */
    float radian1 = radian_arm  + radian_offset ;

    /*
    *  RADIANVALUE   ---->  SERVOVALUE
    */
    //~ float span_s = fabs( RADIANVALUE_S0_LEFT - RADIANVALUE_S0_RIGHT );
    //~ float span_d = fabs( SERVOVALUE_S0_LEFT - SERVOVALUE_S0_RIGHT );
    //~ float value  =   (RADIANVALUE_S0_RIGHT + radian_center )
                   //~ * (span_d / span_s)
                   //~ - (SERVOVALUE_S0_RIGHT);
          //~ value = -value;
    float value = covertRadianToServoValues(RADIANVALUE_S0_LEFT,RADIANVALUE_S0_RIGHT,SERVOVALUE_S0_LEFT,SERVOVALUE_S0_RIGHT,radian1);
    cout << "Servo0  Winkel:" << (radian1*180/PI) << " Servovalue:" << value << endl;
    return value;
}



float Actionfield::getS1ServoValue(const float O_P_x, const float O_P_y, const float O_P_z)
{
    /*
    * translate coordinates to rotation center of servo1 and
    * project them onto servo0 ( easier calculations )
    */
    float S0_P_x = O_P_x - this->mOffsetS0X;
    float S0_P_y = O_P_y - this->mOffsetS0Y;
    float S1_P_z = O_P_z - this->mOffsetS0Z - OFFSET_SERVO1_Z;


    /*
    * calculate distance from rotation center Servo1  to (x,y)
    * --"---          and           to ( (x,y)  , z )
    */
      // from servo0
    float len_S0_P = sqrt( S0_P_x*S0_P_x + S0_P_y*S0_P_y );
    float dist_offset = len_S0_P - sqrt(len_S0_P*len_S0_P - OFFSET_SERVO1_X*OFFSET_SERVO1_X);
    float dist =  len_S0_P - OFFSET_SERVO1_Y - dist_offset;
    float distance_direct_sqr = ( dist*dist + S1_P_z*S1_P_z );
    float distance_direct = sqrt(distance_direct_sqr);



    float relative_height = LENGTH_ARM3 + S1_P_z;
    float dist_servo2_sqr = relative_height*relative_height + dist*dist;
    float dist_servo2     = sqrt( dist_servo2_sqr );


    /*
    *  tries to calculate an vertical adjustment with arm3
    */
    float radian2_1 = atan( relative_height / dist);
    float radian2_2 = acos( ((LENGTH_ARM2*LENGTH_ARM2)-(LENGTH_ARM1*LENGTH_ARM1)-dist_servo2_sqr)
                /  (-2.0 * dist_servo2 * LENGTH_ARM1) );
 	float radian2 = radian2_1 + radian2_2;



     if( isnan(radian2) )
     {
#ifdef __DEBUG__
        cerr << "ERROR: S1 DIVISION!\n" << endl;
#endif
        radian2_1 = atan( S1_P_z / dist ) ;
        float LENGTH_ARM1and2 = LENGTH_ARM2 + LENGTH_ARM1;
        radian2_2 = acos( ((LENGTH_ARM3*LENGTH_ARM3)-(LENGTH_ARM1and2*LENGTH_ARM1and2)- distance_direct_sqr)
                /  (-2.0 * distance_direct * LENGTH_ARM1and2) );
	radian2 = radian2_1 + radian2_2;
     }

    /*
    *  RADIANVALUE   ---->  SERVOVALUE
    */
    //~ float span_s = fabs( RADIANVALUE_S1_LEFT - RADIANVALUE_S1_RIGHT );
    //~ float span_d = fabs( SERVOVALUE_S1_LEFT - SERVOVALUE_S1_RIGHT );
    //~ float value  =   (RADIANVALUE_S1_LEFT + radian2 )
                   //~ * (span_d / span_s)
                   //~ + (SERVOVALUE_S1_LEFT);
    float value = covertRadianToServoValues(RADIANVALUE_S1_LEFT,RADIANVALUE_S1_RIGHT,SERVOVALUE_S1_LEFT,SERVOVALUE_S1_RIGHT,radian2);
    cout << "Servo1  Winkel:" << (radian2*180/PI) << " Servovalue:" << value << endl;
    return value;
}

float Actionfield::getS2ServoValue(const float O_P_x, const float O_P_y, const float O_P_z)
{
    /*
    * translate coordinates to rotation center of servo1 and
    * project them onto servo0 ( easier calculations )
    */
    float S0_P_x = O_P_x - this->mOffsetS0X;
    float S0_P_y = O_P_y - this->mOffsetS0Y;
    float S1_P_z = O_P_z - this->mOffsetS0Z - OFFSET_SERVO1_Z;

    /*
    * calculate distance from rotation center Servo1  to (x,y)
    * --"---          and           to ( (x,y)  , z )
    */
    float len_S0_P = sqrt( S0_P_x*S0_P_x + S0_P_y*S0_P_y );
    float dist_offset = len_S0_P - sqrt(len_S0_P*len_S0_P - OFFSET_SERVO1_X*OFFSET_SERVO1_X);
    float dist =  len_S0_P - OFFSET_SERVO1_Y - dist_offset;

    float relative_height = LENGTH_ARM3 + S1_P_z;
    float dist_servo2_sqr = relative_height*relative_height + dist*dist;
    float dist_servo2     = sqrt( dist_servo2_sqr );

    /*
    *
    */
    float radian3 = PI - acos( (dist_servo2_sqr-(LENGTH_ARM2*LENGTH_ARM2)-(LENGTH_ARM1*LENGTH_ARM1))
                /  (-2.0 * LENGTH_ARM2 * LENGTH_ARM1) );
     if( isnan(radian3) )
     {
#ifdef __DEBUG__
        cerr << "ERROR: S2 DIVISION!\n" << endl;
#endif
        radian3 = 0.0;
     }
    /*
    *  RADIANVALUE   ---->  SERVOVALUE
    */
    //~ float span_s = fabs( RADIANVALUE_S2_LEFT - RADIANVALUE_S2_RIGHT );
    //~ float span_d = fabs( SERVOVALUE_S2_LEFT - SERVOVALUE_S2_RIGHT );
    //~ float value  =   (RADIANVALUE_S2_RIGHT + radian3 )
                   //~ * (span_d / span_s)
                   //~ + (SERVOVALUE_S2_RIGHT);

    float value = covertRadianToServoValues(RADIANVALUE_S2_LEFT,RADIANVALUE_S2_RIGHT,SERVOVALUE_S2_LEFT,SERVOVALUE_S2_RIGHT,radian3);
    cout << "Servo2  Winkel:" << (radian3*180/PI) << " Servovalue:" << value << endl;
    return value;
}

float Actionfield::getS3ServoValue(const float O_P_x, const float O_P_y, const float O_P_z)
{
    /*
    * translate coordinates to rotation center of servo1 and
    * project them onto servo0 ( easier calculations )
    */
    float S0_P_x = O_P_x - this->mOffsetS0X;
    float S0_P_y = O_P_y - this->mOffsetS0Y;
    float S1_P_z = O_P_z - this->mOffsetS0Z - OFFSET_SERVO1_Z;

    /*
    * calculate distance from rotation center Servo1  to (x,y)
    * --"---          and           to ( (x,y)  , z )
    */

    /* from servo0 */
    float len_S0_P = sqrt( S0_P_x*S0_P_x + S0_P_y*S0_P_y );
    float dist_offset = len_S0_P - sqrt(len_S0_P*len_S0_P - OFFSET_SERVO1_X*OFFSET_SERVO1_X);
    float dist =  len_S0_P - OFFSET_SERVO1_Y - dist_offset;
    float distance_direct_sqr = ( dist*dist + S1_P_z*S1_P_z );
    float distance_direct = sqrt(distance_direct_sqr);

    float relative_height = LENGTH_ARM3 + S1_P_z;
    float dist_servo2_sqr = relative_height*relative_height + dist*dist;
    float dist_servo2     = sqrt( dist_servo2_sqr );
    /*
    *
    */
    float radian4_1 = acos( ((distance_direct_sqr)-(LENGTH_ARM3*LENGTH_ARM3)-dist_servo2_sqr)
                /  (-2.0 * dist_servo2 * LENGTH_ARM3) );
    float radian4_2 = acos( ((LENGTH_ARM1*LENGTH_ARM1)-(LENGTH_ARM2*LENGTH_ARM2)-dist_servo2_sqr)
                /  (-2.0 * dist_servo2 * LENGTH_ARM2) );

    float radian4 = PI - radian4_1 - radian4_2;
     if( isnan(radian4) )
     {
#ifdef __DEBUG__
        cerr << "ERROR: S3 DIVISION!\n" << endl;
#endif
        float LENGTH_ARM1and2 = LENGTH_ARM1 + LENGTH_ARM2;
        float err_radian4 = acos( (distance_direct_sqr - (LENGTH_ARM3*LENGTH_ARM3) - (LENGTH_ARM1and2*LENGTH_ARM1and2) )
		/ (-2.0 * LENGTH_ARM3  * LENGTH_ARM1and2 ) );
        radian4 = PI - err_radian4;
     }


    /*
    *  RADIANVALUE   ---->  SERVOVALUE
    */
    //~ float span_s = fabs( RADIANVALUE_S3_LEFT - RADIANVALUE_S3_RIGHT );
    //~ float span_d = fabs( SERVOVALUE_S3_LEFT - SERVOVALUE_S3_RIGHT );
    //~ float value  =   (RADIANVALUE_S3_RIGHT + radian4 )
                   //~ * (span_d / span_s)
                   //~ - (SERVOVALUE_S3_RIGHT);

    float value = covertRadianToServoValues(RADIANVALUE_S3_LEFT,RADIANVALUE_S3_RIGHT,SERVOVALUE_S3_LEFT,SERVOVALUE_S3_RIGHT,radian4);
    cout << "Servo3  Winkel:" << (radian4*180/PI) << " Servovalue:" << value << endl;
    return value;
}


float Actionfield::getS4ServoValue(const bool fingerclosed) // 1: Finger zu, 0: Finger auf   	
{
	if (fingerclosed)
		return SERVOVALUE_S4_RIGHT; 
	else 
		return SERVOVALUE_S4_LEFT;
}



void SleepMs(int ms) {
	usleep(ms*1000); //convert to microseconds
}


bool Actionfield::moveFingersToPos(const float x , const float y, const float z,
                                      float speedfactor)
{
 	float servo0 = this->getS0ServoValue(x, y, z);
    float servo1 = this->getS1ServoValue(x, y, z);
    float servo2 = this->getS2ServoValue(x, y, z);
    float servo3 = this->getS3ServoValue(x, y, z);
    
    mBoard->servo[0].setPos(servo0);
    mBoard->servo[1].setPos(servo1);
    mBoard->servo[2].setPos(servo2);
    mBoard->servo[3].setPos(servo3);
	mBoard->updateSimultaneously(time_to_move*speedfactor); 	//time in seconds to finish move
 
	/* mBoard->update();*/
   	sleep(time_to_wait/(speedfactor*1000));

    return true;
}

int Actionfield::closeFingers() {
	float speedfactor = .5; 	
	mBoard->servo[4].setPos(SERVOVALUE_S4_LEFT);
	mBoard->updateSimultaneously(time_to_move*speedfactor);	
	return 0;
}

int Actionfield::openFingers() {
	float speedfactor = .5; 	
	mBoard->servo[4].setPos(SERVOVALUE_S4_RIGHT);
	mBoard->updateSimultaneously(time_to_move*speedfactor);	
	return 0;
}


bool Actionfield::pickUpAtPos(float distance, float speedfactor) {
	float servo4 = this->getS4ServoValue(distance);
	mBoard->servo[4].setPos(servo4);
	mBoard->updateSimultaneously(time_to_move*speedfactor);	
	sleep(time_to_wait/(speedfactor*1000));
	return true;
}


bool Actionfield::releaseAtPos(float distance, float speedfactor) {
	float servo4 = this->getS4ServoValue(distance);
	mBoard->servo[4].setPos(servo4);
	mBoard->updateSimultaneously(time_to_move*speedfactor);
	sleep(time_to_wait/(speedfactor*1000));
	return true;
}



bool Actionfield::resetArm()	{
	for (int i=0; i < 5; i++)
	{
		mBoard->servo[i].setPos(0.5);	
		mBoard->servo[i].setSpeed(0.95);
	} 
	mBoard->update(); 
	sleep(1);
	mBoard->update(); 
	sleep(1);
	return true;
}


bool Actionfield::hideArm() {
	return this->moveFingersToPos(mOffsetS0X, LENGTH_ARM2-mOffsetS0Y-OFFSET_SERVO1_Y , mOffsetS0Z+OFFSET_SERVO1_Z+LENGTH_ARM1-LENGTH_ARM3, 1.5);
}


void Actionfield::parkArm() {
    this->hideArm();
    mBoard->all_stop();
    sleep(1);
}


