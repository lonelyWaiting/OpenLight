#pragma once

#include "Spectrum/Spectrum.h"
#include "Math/Vector3.h"
#include "Math/Point2.h"

class BxDF
{
public:
	virtual ~BxDF();

	virtual Spectrum f( const Vector3f& wo , const Vector3f& wi ) const = 0;

	// 给定出射方向，以及二维采样点，用于生成随机入射方向
	// 计算方向对的pdf，返回值为该点处，该方向对所对应的BRDF
	virtual Spectrum Sample_f( const Vector3f& wo , Vector3f* wi , const Point2f& samplePoint , float* pdf ) const;

	// 给定出射方向，一组入射方向采样点，用于生成一组随机入射方向
	// 
	virtual Spectrum rho( const Vector3f& wo , int nSamples , Point2f* samples ) const;

	virtual Spectrum rho( int nSamples , Point2f* Samples1 , Point2f* Samples2 ) const;

	// 该组方向对所对应的概率密度值
	virtual float PDF( const Vector3f& wi , const Vector3f& wo ) const;
};