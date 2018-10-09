#pragma once

#include <algorithm>
#include "Math/Vector3.h"

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
inline bool Refract( const Vector3f& w , const Vector3f& n , float eta , Vector3f* wt , bool entering )
{
	float CosThetaI = AbsDot( w , n );

	float SinThetaI2 = MAX( float( 0 ) , 1 - CosThetaI * CosThetaI );
	float sinThetaT2 = eta * eta * SinThetaI2;

	if( sinThetaT2 >= 1.0 )
	{
		// ȫ�ڷ���
		return false;
	}

	float Flip = entering ? 1.0f : -1.0f;

	*wt = Normalize( eta * -w + Flip * n * ( eta * CosThetaI - std::sqrt( 1 - sinThetaT2 ) ) );

	return true;
}