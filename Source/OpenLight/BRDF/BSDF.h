#pragma once

#include "BxDFHelper.h"
#include "Spectrum/Spectrum.h"

class BxDF;

template<typename T>
class Vector3;
typedef Vector3<float> Vector3f;

template<typename T>
class Point2;
typedef Point2<float> Point2f;

class BSDF
{
public:
	BSDF();

	void Add( BxDF* bxdf );

	int Count() const;

	int Count( BxDFType type ) const;

	Spectrum f( const Vector3f& wo , const Vector3f& wi , const Vector3f& n , const BxDFType type );

	Spectrum Sample_f( const Vector3f&wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , BxDFType type , bool& bNoOccur , BxDFType* SampledBxDFType = nullptr ) const;

	float PDF( const Vector3f& wo , const Vector3f& wi , const Vector3f& n , BxDFType type ) const;

	~BSDF();

private:
	int nBxDF;
	static const int nMaxBXDF = 8;
	BxDF* bxdfs[nMaxBXDF];
};