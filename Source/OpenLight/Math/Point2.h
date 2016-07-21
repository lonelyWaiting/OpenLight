#ifndef POINT2_H
#define POINT2_H

#include "MathHelper.h"

template<typename T>
class Vector2;

template<typename T>
class Point2
{
public:
	Point2();


	Point2( T _x , T _y );

	explicit Point2( const Vector2<T> &p );

	Point2( const Point2 &p );

	Point2 & operator = ( const Point2 &p );

public:
	Point2 operator + ( const Vector2<T> &v ) const;

	Point2 operator + ( const Point2 &p ) const;

	Point2& operator += ( const Vector2<T> &v );

	Vector2<T> operator - ( const Point2 & v ) const;

	Point2 operator - ( const Vector2<T> &v ) const;

	Point2& operator -= ( const Vector2<T> &v );

	Point2 operator - () const;

	Point2 operator * ( T f ) const;

	Point2& operator *= ( T f );

	Point2 operator / ( T f ) const;

	Point2 operator /= ( T f );

	T operator[] ( int index ) const;

	T& operator[] ( int index );

	bool operator == ( const Point2 &p ) const;

	bool operator != ( const Point2 &p ) const;

public:
	template<typename D>
	friend D Distance( Point2<D>& p1 , Point2<D>& p2 );

	template<typename D>
	friend D DistanceSq( const Point2<D> &p1 , const Point2<D> &p2 );

	template<typename D>
	friend Point2<D> operator* ( D f , const Point2<D> &p );

	template<typename D>
	friend Point2<D> Floor( const Point2<D> &p );

	template<typename D>
	friend Point2<D> Ceil( const Point2<D> &p );

	template<typename D>
	friend Point2<D> Lerp( D t , const Point2<D> &p0 , const Point2<D> &p1 );

	template<typename D>
	friend Point2<D> Min( const Point2<D> &p0 , const Point2<D> &p1 );

	template<typename D>
	friend Point2<D> Max( const Point2<D> &p0 , const Point2<D> &p1 );
public:
	T x , y;
};

#include "Point2.inl"

#endif