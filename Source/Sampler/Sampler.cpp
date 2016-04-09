#include "PCH.h"
#include "Sampler.h"

Sampler::Sampler()
{
	NumSamples = 1;
	NumSets = 83;
	SamplePos = 0;

	SetupShuffledIndices();
}

Sampler::Sampler( const int num )
{
	NumSamples = num;

	NumSets = 83;

	SamplePos = 0;

	SetupShuffledIndices();
}

Sampler::Sampler( const int num , const int SetNum )
{
	NumSamples = num;
	NumSets = SetNum;

	SamplePos = 0;

	SetupShuffledIndices();
}

Sampler::Sampler( const Sampler& s )
{
	NumSamples = s.NumSamples;

	NumSets = s.NumSets;

	SamplePoints = s.SamplePoints;

	ShuffledIndices = s.ShuffledIndices;

	SamplePos = s.SamplePos;
}

Sampler& Sampler::operator = ( const Sampler& rhs )
{
	if( this == &rhs )
	{
		return *this;
	}

	NumSamples = rhs.NumSamples;
	NumSets = rhs.NumSets;
	SamplePoints = rhs.SamplePoints;
	ShuffledIndices = rhs.ShuffledIndices;
	SamplePos = rhs.SamplePos;
}

Sampler::~Sampler()
{

}

void Sampler::SetNumSets( const int Nums )
{
	NumSets = Nums;
}

int Sampler::GetNumSamples()
{
	return NumSamples;
}

void Sampler::SetupShuffledIndices()
{
	ShuffledIndices.reserve( NumSets * NumSamples );
	std::vector<int> Indices;

	for( int j = 0; j < NumSamples; j++ )
	{
		Indices.push_back( j );
	}

	// ShuffledIndices中存放了NumSets组采样点集
	// 每个采样点集中的值为0~NumSamples的一个随机排列
	for( int i = 0; i < NumSets; i++ )
	{
		random_shuffle( Indices.begin() , Indices.end() );

		for( int j = 0; j < NumSamples; j++ )
		{
			ShuffledIndices.push_back( Indices[j] );
		}
	}
}

CameraSample Sampler::GetSamplePoint()
{
	int Jump = 0;

	if( SamplePos % NumSamples == 0 )
	{
		Jump = ( rand() % NumSets ) * NumSamples;
	}

	return *SamplePoints[Jump + ShuffledIndices[Jump + ( SamplePos++ % NumSamples )]];
}