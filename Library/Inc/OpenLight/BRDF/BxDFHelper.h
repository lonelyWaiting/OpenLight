#pragma once

#include <iostream>
#include <algorithm>
#include "Math/Vector3.h"
#include "Math/Normal.h"

enum BxDFType
{
	REFLECTION = 1 << 0 ,
	TRANSMISSION = 1 << 1 ,
	SPECULAR = 1 << 2 ,
	DIFFUSE = 1 << 3 ,

	ALL_TYPE = REFLECTION | TRANSMISSION | SPECULAR | DIFFUSE ,
};

// compute refraction ray 
// eta = etaI / etaT
inline bool Refract( const Vector3f& wi , const Normal& n , double eta , Vector3f* wt , bool entering )
{
	double CosThetaI = AbsDot( wi , n );

	double SinThetaI2 = 1 - CosThetaI * CosThetaI;
	SinThetaI2 = SinThetaI2 > 0.0 ? SinThetaI2 : 0.0;
	double SinThetaT2 = eta * eta * SinThetaI2;

	if( SinThetaT2 >= 1.0 )
	{
		// È«ÄÚ·´Éä
		return false;
	}

	if( entering )
	{
		*wt = Normalize( eta * wi + n * ( eta * CosThetaI - std::sqrt( 1 - SinThetaT2 ) ) );
	}
	else
	{
		*wt = Normalize( eta * wi - n * ( eta * CosThetaI - std::sqrt( 1 - SinThetaT2 ) ) );
	}

	return true;
}