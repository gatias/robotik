#ifndef INVERSE_H
#define INVERSE_H
#include "roboarm.h"
class Inverse{
	public: 
		static const int ANGLE_ALPHA=0;
		static const int ANGLE_BETA=1;
		static const int ANGLE_GAMMA=2;
		static const int ANGLE_DELTA=3;
		static const int ANGLE_EPSILON=4;
		static const int ANGLE_ZETA=5;

		static const int BONE_SHOULDER=0;
		static const int BONE_HUMERUS=1;
		static const int BONE_ELL=2;
		static const int BONE_HAND=3;

		static const float PI=3.1415968;


		Inverse();
		virtual ~Inverse();

		float getAngle(int angle);
		void setPosition(float x, float y, float z);
		void setClawPosition(float lx, float ly, float rx, float ry);

		void setBounding(int angle, int min, int max);
		void calcAngles();
		void calcClawAngles();
		void setBoneLength(int bone, float length);

		float deg2rad(float deg);
		float rad2deg(float rad);

		void setRoboarm(Roboarm* arm);

		void setAngle(int angle, int deg);

	private:
		float alpha;
		float beta;
		float gamma;
		float delta;
		float epsilon;
		float zeta;

		float alphaMax;
		float alphaMin;
		float betaMax;
		float betaMin;
		float gammaMax;
		float gammaMin;
		float deltaMax;
		float deltaMin;


		float targetX;
		float targetY;
		float targetZ;

		float targetClawlX;
		float targetClawlY;
		float targetClawrX;
		float targetClawrY;
		Roboarm* roboarm;
		
};
#endif
