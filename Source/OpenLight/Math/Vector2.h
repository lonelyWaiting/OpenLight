#ifndef VECTOR2_H
#define VECTOR2_H

#include "MathHelper.h"

template<typename T>
class Point2;

template<typename T>
class Point3;

template<typename T>
class Vector2
{
public:
	Vector2();

	Vector2( T _x , T _y );

	explicit Vector2( const Point2<T> &p );

	explicit Vector2( const Point3<T> &p );

	Vector2( const Vector2 &rhs );

	Vector2& operator = ( const Vector2 &rhs );

	Vector2 operator + ( const Vector2& v ) const;

	Vector2& operator += ( const Vector2& v );

	Vector2 operator - ( const Vector2 &v ) const;

	Vector2& operator -= ( const Vector2 &v );

	bool operator == ( const Vector2 &v ) const;

	bool operator != ( const Vector2 & v ) const;

	Vector2 operator * ( const Vector2& v ) const;

	Vector2 operator* ( T f ) const;

	Vector2& operator *= ( T f );

	Vector2 operator / ( T f ) const;

	Vector2& operator /= ( T f );

	Vector2 operator - () const;

	T& operator [] ( int index );

	T LengthSq() const;

	T Length() const;

public:
	template<typename D>
	friend Vector2<D> operator* ( D f , const Vector2<D> &v );

	template<typename D>
	friend D Dot( const Vector2<D> &v1 , const Vector2<D> &v2 );

	template<typename D>
	friend D AbsDot( const Vector2<D> &v1 , const Vector2<D> &v2 );

	template<typename D>
	friend Vector2<D> Normalize( const Vector2<D> &v );

	template<typename D>
	friend Vector2<D> Abs( const Vector2<D> &v );

public:
	T x , y;
};

#include "Vector2.inl"
#endif