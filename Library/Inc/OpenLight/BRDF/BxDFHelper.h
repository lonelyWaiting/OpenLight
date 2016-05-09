#pragma once

#include <algorithm>
#include "Math/Vector3.h"
#include "Math/Normal.h"

// compute refraction ray 
// eta = etaI / etaT
inline bool Refract( const Vector3f& wi , const Normal& n , double eta , Vector3f* wt , bool entering )
{
	double CosThetaI = AbsDot( wi , n );

	double SinThetaI2 = std::max( double( 0 ) , 1 - CosThetaI * CosThetaI );
	double sinThetaT2 = eta * eta * SinThetaI2;

	if( sinThetaT2 >= 1.0 )
	{
		// È«ÄÚ·´Éä
		return false;
	}

	if( entering )
	{
		*wt = Normalize( eta * wi + n * ( eta * CosThetaI - std::sqrt( 1 - sinThetaT2 ) ) );
	}
	else
	{
		*wt = Normalize( eta * wi - n * ( eta * CosThetaI - std::sqrt( 1 - sinThetaT2 ) ) );
	}

	return true;
}