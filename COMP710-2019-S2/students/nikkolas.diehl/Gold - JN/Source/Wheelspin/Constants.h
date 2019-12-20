#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
//#define _USE_MATH_DEFINES

//Libraries
//#include <math.h>
#include "Box2D/Common/b2Math.h"

const double HALF_PI = b2_pi / 2;
const double PI = b2_pi;
const double QUARTER_PI = b2_pi / 4;
const double TAU = b2_pi * 2;
const double CHECK_INACCURACY = 1;



class Constants
{
public:
	Constants();
	~Constants();
};

#endif

