#pragma once

#include "BxDFHelper.h"
#include "Spectrum/Spectrum.h"

class BxDF;
class Vector3f;
class Normal;
class Point2f;

class BSDF
{
public:
	BSDF();

	void Add( BxDF* bxdf );

	int Count() const;

	int Count( BxDFType type ) const;

	Spectrum f( const Vector3f& wo , const Vector3f& wi , const Normal& n , const BxDFType type );

	Spectrum Sample_f( const Vector3f&wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , BxDFType type , bool& bNoOccur , BxDFType* SampledBxDFType = nullptr ) const;

	double PDF( const Vector3f& wo , const Vector3f& wi , const Normal& n , BxDFType type ) const;

//private:
	~BSDF();

private:
	int nBxDF;
	static const int nMaxBXDF = 8;
	BxDF* bxdfs[nMaxBXDF];
};