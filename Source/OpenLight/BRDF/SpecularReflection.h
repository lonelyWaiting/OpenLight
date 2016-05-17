#pragma once

#include "BxDF.h"
#include "Fresnel.h"

class SpecularReflection : public BxDF
{
public:
	SpecularReflection( const Spectrum& R , const Fresnel* fresnel );

	~SpecularReflection();

	Spectrum f( const Vector3f& wo , const Vector3f& wi ) const;

	Spectrum Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , bool& bNoOccur ) const;

	double PDF( const Vector3f& wi , const Vector3f& wo , const Normal& n ) const;

private:
	const Spectrum R;			// 用于缩放反射颜色
	const Fresnel* fresnel;	// 用于计算入射光被反射的比例
};