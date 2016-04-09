#ifndef POINT3_H
#define POINT3_H

#include <ostream>

class Vector3f;

class Point3f
{
public:
	Point3f();

	Point3f( float _x , float _y , float _z );

	Point3f( const Point3f &p );

	Point3f& operator = ( const Point3f& p );

public:
	friend std::ostream & operator<< ( std::ostream & os , const Point3f& p );

public:
	Point3f operator + ( const Vector3f& v ) const;

	Point3f operator + ( const Point3f& p ) const;

	Point3f& operator += ( const Vector3f& v );

	Vector3f operator - ( const Point3f& p ) const;

	Point3f operator - ( const Vector3f& v ) const;

	Point3f& operator -= ( const Vector3f& p );

	Point3f operator* ( float f ) const;

	Point3f& operator *= ( float f );

	Point3f operator / ( float f ) const;

	Point3f& operator /= ( float f );

	float operator[] ( int index ) const;

	float & operator [] ( int i );

	bool operator == ( const Point3f& p ) const;

	bool operator != ( const Point3f& p ) const;

public:
	friend inline float Distance( const Point3f &p1 , const Point3f &p2 );

	friend inline float DistanceSq( const Point3f &p1 , const Point3f &p2 );

	friend inline Point3f operator*( float f , const Point3f &p );

	friend Point3f Lerp( float t , const Point3f &p0 , const Point3f& p1 );

	template<typename T>
	friend Point3f Min( const Point3f &p1 , const Point3f &p2 );

	template<typename T>
	friend Point3f Max( const Point3f &p1 , const Point3f &p2 );

	template<typename T>
	friend Point3f Floor( const Point3f &p );

	template<typename T>
	friend Point3f Ceil( const Point3f &p );

	template<typename T>
	friend Point3f Abs( const Point3f& p );

	template<typename T>
	friend Point3f Permute( const Point3f &p , int x , int y , int z );
public:
	float x , y , z;
};

#endif