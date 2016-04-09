#pragma once

#include "Vector3.h"

class Normal
{
public:
	Normal();

	Normal( float x , float y , float z );

	explicit Normal( const Vector3f& v );

	Normal operator - () const;

	Normal operator + ( const Normal& n ) const;

	Normal& operator += ( const Normal& n );

	Normal operator - ( const Normal& n ) const;

	Normal& operator -= ( const Normal& n );

	Normal operator * ( float f ) const;

	Normal& operator *= ( float f );

	Normal operator / ( float f ) const;

	Normal& operator /= ( float f );

	float operator[]( int i ) const;

	float& operator[] ( int i );

	bool operator == ( const Normal& n ) const;

	bool operator != ( const Normal& n ) const;

	float operator * ( const Vector3f& v );

	float LengthSquared() const;

	float Length() const;

	bool HasNAN() const;

	friend float Dot( const Vector3f& v , const Normal& n );

	friend float Dot( const Normal& n , const Vector3f& v );

	friend float AbsDot( const Normal& n , const Vector3f& v );

public:
	float x , y , z;
};