#pragma once

#ifndef SOLID_NOISE_H
#define SOLID_NOISE_H

#include "Math/Vector3.h"

class SolidNoise
{
public:
	SolidNoise();

public:
	double noise( const Vector3f& ) const;

	double turbulence( const Vector3f& p , int depth ) const;

	double dturbulence( const Vector3f& p , int depth , double d ) const;

	double omega( double t ) const;

	Vector3f gamma( int i , int j , int k ) const;

	int intGamma( int i , int j ) const;

	double knot( int i , int j , int k , Vector3f& v ) const;

public:
	Vector3f grad[16];
	int phi[16];
};
#endif