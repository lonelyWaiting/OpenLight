#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <algorithm>
#include <limits>
#include <math.h>

#define PI	3.1415926535897932384
#define TWO_PI 6.2831853071795864769
#define PI_ON_180 0.0174532925199432957
#define INV_PI	0.3183098861837906715
#define INV_TWO_PI	0.1591549430918953358

#define EPSILON 1e-3
#define EPSILON_04	1e-4

#define Infinity std::numeric_limits<float>::infinity()

namespace srMath
{
	template<typename T>
	bool isNanN( const T x )
	{
		return std::isnan( x );
	}

	template<typename T>
	T ToRadians( T degree )
	{
		return ( degree / 180.0 ) * PI;
	}

	template<typename T>
	T ToDegree( T radian )
	{
		return ( 180.0 * PI ) / radian;
	}

	template<typename T>
	T Lerp( const T& t , const T& p1 , const T& p2 )
	{
		return p1 * ( 1.0 - t ) + p2 * t;
	}

	template<typename T>
	T clamp( T x , T _min , T _max )
	{
		return x < _min ? _min : x > _max ? _max : x;
	}

	template<typename T>
	T floor( T& x )
	{
		return static_cast< int >( x );
	}

	template<typename T>
	T ceil( T& x )
	{
		return ceil( x );
	}

	template<typename T>
	T Min( T x , T y )
	{
		return ( x ) < ( y ) ? ( x ) : ( y );
	}

	template<typename T>
	T Max( T x , T y )
	{
		return ( x ) > ( y ) ? x : y;
	}
}

template<typename T>
class Point2;
typedef Point2<float> Point2f;
typedef Point2<double> Point2l;

template<typename T>
class Point3;
typedef Point3<float> Point3f;
typedef Point3<double> Point3l;

template<typename T>
class Vector2;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2l;

template<typename T>
class Vector3;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3l;

template<typename T>
class Vector4;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4l;

template<typename T>
class Ray;
typedef Ray<double> Rayl;
typedef Ray<float>	Rayf;

template<typename T>
class Matrix3;
typedef Matrix3<double>	Matrix3l;
typedef Matrix3<float>	Matrix3f;

template<typename T>
class Matrix4;
typedef Matrix4<double> Matrix4l;
typedef Matrix4<float>  Matrix4f;

template<typename T>
class Bound2;
typedef Bound2<float>  Bound2f;
typedef Bound2<double> Bound2l;

template<typename T>
class Bound3;
typedef Bound3<double> Bound3l;
typedef Bound3<float>  Bound3f;

template<typename T>
class ONB;
typedef ONB<float> ONBf;
typedef ONB<double> ONBl;
#endif