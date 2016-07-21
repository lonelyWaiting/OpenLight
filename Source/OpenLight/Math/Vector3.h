#ifndef VECTOR3_H
#define VECTOR3_H

#include "MathHelper.h"
#include <cmath>

template<typename T>
class Point3;

template<typename T>
class Vector3
{
public:
	Vector3();

	Vector3( T value );

	Vector3( T _x , T _y , T _z );

	Vector3( const Vector3& rhs );

	explicit Vector3( const Point3<T>& p );

public:
	Vector3& operator = ( const Vector3& rhs );

	Vector3 operator + ( const Vector3& rhs ) const;

	Vector3& operator += ( const Vector3& rhs );

	Vector3 operator - ( const Vector3& rhs ) const;

	Vector3& operator -= ( const Vector3& rhs );

	Vector3 operator * ( T scale ) const;

	Vector3& operator *= ( T scale );

	T operator * ( const Vector3 &rhs ) const;

	Vector3 operator -() const;

	Vector3 operator / ( T f ) const;

	Vector3& operator /= ( T f );

	T& operator [] ( int index );

	T operator[] ( int index ) const;

	bool operator == ( const Vector3& rhs ) const;

	bool operator != ( const Vector3& rhs ) const;

public:
	T LengthSq() const;

	T Length() const;

public:
	template<typename D>
	friend Vector3<D> operator* ( D s , const Vector3<D> &v );

	template<typename D>
	friend Vector3<D> Abs( const Vector3<D> &v );

	template<typename D>
	friend D Dot( const Vector3<D> &v1 , const Vector3<D> &v2 );

	template<typename D>
	friend D Dot( const Vector3<D>& w , const Point3<D>& p );

	template<typename D>
	friend D AbsDot( const Vector3<D> & v1 , const Vector3<D>& v2 );

	template<typename D>
	friend Vector3<D> Cross( const Vector3<D> &v1 , const Vector3<D> &v2 );

	template<typename D>
	friend Vector3<D> Normalize( const Vector3<D>& v );

	template<typename D>
	friend D MinComponent( const Vector3<D> &v );

	template<typename D>
	friend D MaxCompoent( const Vector3<D> &v );

	template<typename D>
	friend Vector3<D> Max( const Vector3<D> &p1 , const Vector3<D> &p2 );

	template<typename D>
	friend Vector3<D> Min( const Vector3<D> &p1 , const Vector3<D> &p2 );

	template<typename D>
	friend Vector3<D> Permute( const Vector3<D> &v , int x , int y , int z );

	template<typename D>
	friend void CoordinateSystem( Vector3<D> &v1 , Vector3<D> *v2 , Vector3<D> *v3 );

	template<typename D>
	friend int MaxLengthComponent( const Vector3<D>& v );

public:
	union
	{
		struct
		{
			T x , y , z;
		};

		struct
		{
			T r , g , b;
		};
	};
};

#include "Vector3.inl"

#endif