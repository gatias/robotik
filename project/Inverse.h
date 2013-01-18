#ifndef INVERSE_H
#define INVERSE_H
#include "roboarm.h"
class Inverse{
	public: 
		static const int ANGLE_ALPHA=0;
		static const int ANGLE_BETA=1;
		static const int ANGLE_GAMMA=2;
		static const int ANGLE_DELTA=3;

		static const int BONE_SHOULDER=4;
		static const int BONE_HUMERUS=5;
		static const int BONE_ELL=6;
		static const int BONE_HAND=7;

		static const float PI=3.1415968;


		Inverse();
		virtual ~Inverse();

		float getAngle(int angle);
		void setPosition(float x, float y, float z);
		void setBounding(int angle, int min, int max);
		void calcAngles();
		void setBoneLength(int bone, float length);

		float deg2rad(float deg);

		void setRoboarm(Roboarm* arm);

	private:
		float alpha;
		float beta;
		float gamma;
		float delta;

		float alphaMax;
		float alphaMin;
		float betaMax;
		float betaMin;
		float gammaMax;
		float gammaMin;
		float deltaMax;
		float deltaMin;

		float shoulderLength;
		float humerusLength;
		float ellLength;
		float handLength;

		float targetX;
		float targetY;
		float targetZ;

		Roboarm* roboarm;
		
};
#endif
