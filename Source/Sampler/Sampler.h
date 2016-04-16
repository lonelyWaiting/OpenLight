#pragma once

#include "PCH.h"
#include "RandomNumberGenerator.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "CameraSample.h"

class Sampler
{
public:
	Sampler();

	Sampler( const Sampler& s );

	Sampler& operator = ( const Sampler& rhs );

	virtual void GenerateUnitSquareSamples() = 0;

public:
	void SetProperty( const int SampleCount , const int SampleGroupCount );

protected:
	void SetupShuffledIndices();

	void ShuffleXCoordinate();

	void ShuffleYCoordinate();

public:
	CameraSample GetSamplePoint();

public:
	virtual void ParseSampler( XMLElement* SamplerRootElement ) = 0;

public:
	std::vector<int>			ShuffledIndices;					// 采样集的数目
	unsigned long				SamplePos;							// 当前使用的采样点数目

	std::vector<CameraSample*> SamplePoints;

	int SampleCount;													// 每组采样点的采样点数目
	int SampleGroupCount;													// 采样组数
};

typedef Sampler* SamplerPtr;