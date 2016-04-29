#pragma once

#include "Vector3.h"

class Normal
{
public:
	Normal();

	Normal( double x , double y , double z );

	explicit Normal( const Vector3f& v );

	operator Vector3f();

	operator Vector3f() const;

	Normal operator - () const;

	Normal operator + ( const Normal& n ) const;

	Normal& operator += ( const Normal& n );

	Normal operator - ( const Normal& n ) const;

	Normal& operator -= ( const Normal& n );

	Normal operator * ( double f ) const;

	Normal& operator *= ( double f );

	Normal operator / ( double f ) const;

	Normal& operator /= ( double f );

	double operator[]( int i ) const;

	double& operator[] ( int i );

	bool operator == ( const Normal& n ) const;

	bool operator != ( const Normal& n ) const;

	double operator * ( const Vector3f& v );

	double LengthSquared() const;

	double Length() const;

	bool HasNAN() const;

	friend double Dot( const Vector3f& v , const Normal& n );

	friend double Dot( const Normal& n , const Vector3f& v );

	friend double AbsDot( const Normal& n , const Vector3f& v );

	friend double AbsDot( const Vector3f& v , const Normal& n );

	friend Normal Normalize( const Normal& n );

public:
	double x , y , z;
};