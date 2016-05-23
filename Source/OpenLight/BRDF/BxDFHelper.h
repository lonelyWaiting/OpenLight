#pragma once

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
inline bool Refract( const Vector3f& w , const Normal& n , double eta , Vector3f* wt , bool entering )
{
	double CosThetaI = AbsDot( w , n );

	double SinThetaI2 = std::max( double( 0 ) , 1 - CosThetaI * CosThetaI );
	double sinThetaT2 = eta * eta * SinThetaI2;

	if( sinThetaT2 >= 1.0 )
	{
		// È«ÄÚ·´Éä
		return false;
	}

	float Flip = entering ? 1.0 : -1.0;

	*wt = Normalize( eta * -w + Flip * n * ( eta * CosThetaI - std::sqrt( 1 - sinThetaT2 ) ) );

	return true;
}