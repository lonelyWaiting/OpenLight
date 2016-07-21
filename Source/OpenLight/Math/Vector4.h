#ifndef VECTOR4_H
#define VECTOR4_H

#include "MathHelper.h"
#include <cmath>

template<typename T>
class Vector3;

template<typename T>
class Vector4
{
public:
	Vector4();

	Vector4( T a );

	Vector4( T x , T y , T z , T w );

	Vector4( const Vector4& v );

	Vector4( const Vector3<T>& v );

public:
	bool operator == ( const Vector4& v ) const;

	bool operator != ( const Vector4& v ) const;

	const Vector4& operator = ( const Vector4& v );

	const Vector4& operator += ( const Vector4& v );

	Vector4 operator + ( const Vector4& v ) const;

	const Vector4& operator -= ( const Vector4& v );

	Vector4 operator - ( const Vector4& v ) const;

	Vector4 operator - () const;

	const Vector4& operator *= ( const Vector4& v );

	const Vector4& operator *= ( T a );

	const Vector4& operator /= ( const Vector4& v );

	const Vector4& operator /= ( T a );

	Vector4 operator / ( const Vector4& v ) const;

	Vector4 operator / ( T a ) const;

public:
	T length() const;

	T lengthSq() const;

	const Vector4& normalize();

	static unsigned int dimension();

public:
	template<typename D>
	friend D Dot( const Vector4<D>&v1 , const Vector4<D> &v2 );

	template<typename D>
	friend D AbsDot( const Vector4<D>& v1 , const Vector4<D>& v2 );

	template<typename D>
	friend Vector4<D> Abs( const Vector4<D>& v );

	template<typename D>
	friend Vector4<D> Permute( const Vector4<D> &v , int x , int y , int z , int w );

	template<typename D>
	friend Vector4<D> Normalize( const Vector4<D>& v );

public:

	union
	{
		struct
		{
			T x , y , z , w;
		};

		struct
		{
			T r , g , b , a;
		};
	};
};

#include "Vector4.inl"
#endif