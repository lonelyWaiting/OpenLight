#ifndef POINT3_H
#define POINT3_H

#include "MathHelper.h"
#include <cmath>

template<typename T>
class Vector3;

template<typename T>
class Point3
{
public:
	Point3();

	Point3( T _x , T _y , T _z );

	Point3( const Point3 &p );

	Point3& operator = ( const Point3& p );

public:
	Point3 operator + ( const Vector3<T>& v ) const;

	Point3 operator + ( const Point3& p ) const;

	Point3& operator += ( const Vector3<T>& v );

	Vector3<T> operator - ( const Point3& p ) const;

	Point3 operator - ( const Vector3<T>& v ) const;

	Point3& operator -= ( const Vector3<T>& p );

	Point3 operator* ( T f ) const;

	Point3& operator *= ( T f );

	Point3 operator / ( T f ) const;

	Point3& operator /= ( T f );

	T operator[] ( int index ) const;

	T & operator [] ( int i );

	bool operator == ( const Point3& p ) const;

	bool operator != ( const Point3& p ) const;

public:
	template<typename D>
	friend D Distance( const Point3<D> &p1 , const Point3<D> &p2 );

	template<typename D>
	friend D DistanceSq( const Point3<D> &p1 , const Point3<D> &p2 );

	template<typename D>
	friend Point3<D> operator*( D f , const Point3<D> &p );

	template<typename D>
	friend Point3<D> Lerp( D t , const Point3<D> &p0 , const Point3<D>& p1 );

	template<typename D>
	friend Point3<D> Min( const Point3<D> &p1 , const Point3<D> &p2 );

	template<typename D>
	friend Point3<D> Max( const Point3<D> &p1 , const Point3<D> &p2 );

	template<typename D>
	friend Point3<D> Floor( const Point3<D> &p );

	template<typename D>
	friend Point3<D> Ceil( const Point3<D> &p );

	template<typename D>
	friend Point3<D> Abs( const Point3<D>& p );

	template<typename D>
	friend Point3<D> Permute( const Point3<D> &p , int x , int y , int z );

	template<typename D>
	friend D operator* ( const Point3<D>& p , const Vector3<D>& v );

public:
	T x , y , z;
};

#include "Point3.inl"
#endif