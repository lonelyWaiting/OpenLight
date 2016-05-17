#pragma once

#include "Spectrum/Spectrum.h"
#include "Math/Vector3.h"
#include "Math/Point2.h"
#include "BxDFType.h"

class BxDF
{
public:
	BxDF( BxDFType type );

	virtual ~BxDF();

	virtual Spectrum f( const Vector3f& wo , const Vector3f& wi ) const = 0;

	// 给定出射方向，以及二维采样点，用于生成随机入射方向
	// 计算方向对的pdf，返回值为该点处，该方向对所对应的BRDF
	virtual Spectrum Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , bool& bNoOccur ) const;

	// 给定出射方向，一组入射方向采样点，用于生成一组随机入射方向
	virtual Spectrum rho( const Vector3f& wo , const Normal& n , int nSamples , Point2f* samples ) const;

	virtual Spectrum rho( const Normal& n , int nSamples , Point2f* Samples1 , Point2f* Samples2 ) const;

	// 该组方向对所对应的概率密度值
	virtual double PDF( const Vector3f& wi , const Vector3f& wo , const Normal& n ) const;

	bool IsMatch( const BxDFType& flag );

public:
	BxDFType type;
};