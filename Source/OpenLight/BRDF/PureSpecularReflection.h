#pragma once

#include "BxDF.h"

class PureSpecularReflection : public BxDF
{
public:
	PureSpecularReflection( const Spectrum& R );

	~PureSpecularReflection();

	Spectrum f( const Vector3f& wo , const Vector3f& wi ) const;

	Spectrum Sample_f( const Vector3f& wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , bool& bNoOccur ) const;

	float PDF( const Vector3f& wi , const Vector3f& wo , const Vector3f& n ) const;

private:
	const Spectrum R;			// 用于缩放反射颜色
};