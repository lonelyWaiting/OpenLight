#pragma once

#include "PCH.h"
#include "RandomNumberGenerator.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "CameraSample.h"

class Sample;

class Sampler
{
public:
	Sampler();

	Sampler( const int num );

	Sampler( const int num , const int num_sets );

	Sampler( const Sampler& s );

	Sampler& operator = ( const Sampler& rhs );

	virtual ~Sampler();

	virtual Sampler* clone() const = 0;

	virtual void GenerateUnitSquareSamples() = 0;

public:
	void SetNumSets( const int num );

	int GetNumSamples();

protected:
	void SetupShuffledIndices();

public:
	CameraSample GetSamplePoint();

public:
	std::vector<int>			ShuffledIndices;					// 采样集的数目
	unsigned long				SamplePos;							// 当前使用的采样点数目

	std::vector<CameraSample*> SamplePoints;

	int NumSamples;													// 每组采样点的采样点数目
	int NumSets;													// 采样组数
};

typedef Sampler* SamplerPtr;