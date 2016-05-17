#pragma once

#include "BxDF.h"

class PureSpecularReflection : public BxDF
{
public:
	PureSpecularReflection( const Spectrum& R );

	~PureSpecularReflection();

	Spectrum f( const Vector3f& wo , const Vector3f& wi ) const;

	Spectrum Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , bool& bNoOccur ) const;

	double PDF( const Vector3f& wi , const Vector3f& wo , const Normal& n ) const;

private:
	const Spectrum R;			// 用于缩放反射颜色
};