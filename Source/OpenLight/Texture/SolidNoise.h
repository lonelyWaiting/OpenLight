#pragma once

#ifndef SOLID_NOISE_H
#define SOLID_NOISE_H

#include "Math/Vector3.h"

class SolidNoise
{
public:
	SolidNoise();

public:
	float noise( const Vector3f& ) const;

	float turbulence( const Vector3f& p , int depth ) const;

	float dturbulence( const Vector3f& p , int depth , float d ) const;

	float omega( float t ) const;

	Vector3f gamma( int i , int j , int k ) const;

	int intGamma( int i , int j ) const;

	float knot( int i , int j , int k , Vector3f& v ) const;

public:
	Vector3f grad[16];
	int phi[16];
};
#endif