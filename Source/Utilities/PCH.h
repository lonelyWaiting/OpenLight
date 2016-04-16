#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>
#include <sstream>
#include <cstdio>
#include <tchar.h>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <limits>
#include <ostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "tinyxml2.h"

using namespace tinyxml2;

using std::cout;
using std::cerr;
using std::endl;

// TODO: reference additional headers your program requires here

#define Assert(expr)	((expr) ? void(0) : Log::Get().Error("Assertion \'%s\' failed in %s , line %d" , #expr , __FILE__ , __LINE__))

#define MIN(x , y)	(x) > (y) ? (y) : (x)
#define MAX(x , y)	(x) > (y) ? (x) : (y)

#define PI	3.1415926535897932384
#define TWO_PI 6.2831853071795864769
#define PI_ON_180 0.0174532925199432957
#define INV_PI	0.3183098861837906715
#define INV_TWO_PI	0.1591549430918953358

#define EPSILON 1e-3
#define EPSILON_04	1e-4

#define Infinity std::numeric_limits<double>::infinity()

#define Clamp(x , _min , _max)	x = (x) < _min ? _min : ((x) > _max ? _max : (x))

#define USE_POSITION	0x1
#define USE_NORMAL		0x2
#define USE_TEXCOORD	0x4

#define SAFE_DELETE(p)	if((p)){delete (p); p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	if((p)){delete[] p; p = nullptr;}

static const double inchToMm = 25.4f;

template<typename T>
inline bool isNanN( const T x )
{
	return std::isnan( x );
}

template<>
inline bool isNanN( const int x )
{
	return false;
}

inline double Lerp( const double& t , const double& p1 , const double& p2 )
{
	return ( 1 - t ) * p1 + p2 * t;
}

inline double ToRadians( double degree )
{
	return ( ( degree / 180.0f ) * PI );
}

inline double ToDegree( double radian )
{
	return ( ( 180.0f / PI ) * radian );
}

template<typename T>
void Swap( T& A , T& B )
{
	T C = A;
	A = B;
	B = C;
}

inline bool Quadtratic( double A , double B , double C , double* t0 , double* t1 )
{
	double delta = B * B - 4 * A * C;

	if( delta < 0 )
	{
		// 不存在实数根
		return false;
	}

	double sqrtDelta = sqrtf( delta );

	double InvA = 1.0f / A;

	*t0 = ( -1.0f * B - 1.0f * sqrtDelta ) * 0.5f * InvA;
	*t1 = ( -1.0f * B + sqrtDelta ) * 0.5f * InvA;

	if( *t0 > *t1 )
	{
		//Swap( *t0 , *t1 );
		double temp = *t0;
		*t0 = *t1;
		*t1 = temp;
	}

	return true;
}