/**
* July 2009 Edward-Robert Tyercha
* March 2012 Edward-Robert Tyercha (Update for silver-arm)
*
**/

#ifndef ACTIONFIELD_H
#define ACTIONFIELD_H

//#define __DEBUG__

#include "servoboard.h"

#define PI M_PI //3.14159265

/// The length of the robotic arm parts
#define LENGTH_ARM1 15.0 // OK
#define LENGTH_ARM2 18.0 // ~OK
#define LENGTH_ARM3 9.0  // OK

/// Distances from servo1 relative to servo0
#define OFFSET_SERVO1_X 0.0  // keine Verschiebung
#define OFFSET_SERVO1_Y 0.0  // keine Verschiebung
#define OFFSET_SERVO1_Z 7.5  // ok

/** This values represent the internal servo-values needed by the servoboard
*   to adjust them to the virtual angles used by our class. Each servo has a
*   left and a right maximum that represents either a 0� 90� or 180� (in radian).
*   Furthermore we need reference values (the ones listed below below).
*
*   EXAMPLE: Let servo0 radian_left be 0� and radian_right be 100� and value_left
*   be 0 and value_right be 10  (10times smaller). When our virtual-arm says
*   that our arm should be rotated so that it points 50� than our servos need
*   to be rotated to 5. (We project kinda'way coordinates from one space into an
*   other.
**/
#define SERVOVALUE_S0_RIGHT 0.945   // right side
#define SERVOVALUE_S0_LEFT  0.070   // left side 320
#define SERVOVALUE_S1_RIGHT 0.690    // ungenau
#define SERVOVALUE_S1_LEFT  0.305   // ungenau
#define SERVOVALUE_S2_RIGHT 0.110   // ungenau
#define SERVOVALUE_S2_LEFT  0.525   // ungenau
#define SERVOVALUE_S3_RIGHT 0.62    // ok
#define SERVOVALUE_S3_LEFT  0.145   // strange
#define SERVOVALUE_S4_RIGHT 0.52  // greifer offen
#define SERVOVALUE_S4_LEFT  0.9  //  geschlossen
//~ #define SERVOVALUE_S5_RIGHT 0.2  
//~ #define SERVOVALUE_S5_LEFT  0.5  


#define RADIANVALUE_S0_RIGHT 0.0        //ok
#define RADIANVALUE_S0_LEFT  PI        //ok
#define RADIANVALUE_S1_RIGHT 3.0*PI/4.0     //ok
#define RADIANVALUE_S1_LEFT  PI/4.0        //ok
#define RADIANVALUE_S2_RIGHT 0          //ok
#define RADIANVALUE_S2_LEFT  PI/2.0     //ok
#define RADIANVALUE_S3_RIGHT 0.0        //ok
#define RADIANVALUE_S3_LEFT  PI/2.0     //ok
//~ #define RADIANVALUE_S5_RIGHT 0.0        //ok
//~ #define RADIANVALUE_S5_LEFT  PI/2.0     //ok




/** This class represents a virtual playground for our arm. The Actionfield
*   connects to the servoboard and initialises 7 (from 0 to 6) servos. It offers
*   methods to grab a figure to a given (x,y,z) location. It's implemented to
*   let the arm always be vertical aligned to the playground so it can easily
*   pick up figures without tipping off surrounding figures.
*
*   Each table playing game should implement it's own adapter class that uses
*   this class to move the arm. (see chessfield class).
**/
class Actionfield
{

/*
     width
^----------+
|          |
| y        |  height
|          |
|    x     |
O---------->
|<-offsetS0Y
|
+---(S0)
  ^
 offsetS0X
*/

public:
        /**
        * Default constructor creates our specified field.
        **/
        Actionfield();
        
        /**
        * Constructor creates our specified field on given device.
        **/
        Actionfield(char* dev);

		/**
		* This method initialises the actionfield internal data
		**/
		void init(char* dev);

        /**
        * These methods calculate the servo values which later are sent to the
        * servoboard. For futher information read the documentation.
        * @param x,y,z are relative to the actionfield zero.
        **/
        float getS0ServoValue(const float x , const float y, const float z);
        float getS1ServoValue(const float x , const float y, const float z);
        float getS2ServoValue(const float x , const float y, const float z);
        float getS3ServoValue(const float x , const float y, const float z);
        float getS4ServoValue(const bool );

        /**
        * This method moves the arm to a given 3D point so that the finger-tips
        * point vertical onto them. This point doesn't need to be within the
        * given action field (width-height), but be aware::: -->
        *
        * ! WARNING ! the arm may broke if the point
        * is physically pretty near the basement or servo0.
        *
        * @param x,y,z  Point
        * @param speedfactor Float
        */
        bool moveFingersToPos(const float x , const float y, const float z,  float speedfactor = 1.0);

        /**
        * This method resets the arm to a nearly natural position at the center
        * of the actionfield with the height of servo0.
        * -> This method uses moveFingersToPos( ... );
        **/
        bool resetArm(); //places arm at center pos

        /**
        * This method moves the arm to a position where the arm occupoes a
        * minimum of space (folded backwars fith the fingers near to servo0.
        * Use this method to "hide" the arm from the actionfield, so that the
        * field isn't covered by the arm.
        * -> This method uses moveFingersToPos( ... );

        **/
        bool hideArm(); 


		/**
		* Picks up stone by closing fingers to value of distance
		* Releases stone by opening fingers to value of distance 
		* ->basicly the same methods, so be careful with the value of distance
		**/
		bool pickUpAtPos(float distance, float speedfactor);
		
		bool releaseAtPos(float distance, float speedfactor); 
		
		int closeFingers();
		
		int openFingers();

        /**
        * This method moves the arm to a preconfigured position where it is
        * placed save when the programm shutsdown and the energy is shut down.
        *
        **/
	    void parkArm();  // moves the arm to an predefined position before shutdown.

        /**
        * Destructor
        */
        virtual ~Actionfield();
        
        /**
         * Quick access method
         */
        inline Servoboard* getServoboard() { return mBoard; }

private:
		float time_to_wait;
		int time_to_move;
		
        /// This member variable saves the width of the action field.
        float mFieldWidth;

        /// This member variable saves the height of the action field.
        float mFieldHeight;

        /// This member variables save the relative distances to servo0.
        float mOffsetS0X;
        float mOffsetS0Y;
        float mOffsetS0Z;

        /// The communication interface with the physical board.
        Servoboard* mBoard;

};

#endif // ACTIONFIELD_H
