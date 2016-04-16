#include "PCH.h"
#include "Sampler.h"

Sampler::Sampler()
{

}

Sampler::Sampler( const Sampler& s )
{
	SampleCount = s.SampleCount;

	SampleGroupCount = s.SampleGroupCount;

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

	SampleCount = rhs.SampleCount;
	SampleGroupCount = rhs.SampleGroupCount;
	SamplePoints = rhs.SamplePoints;
	ShuffledIndices = rhs.ShuffledIndices;
	SamplePos = rhs.SamplePos;
}

void Sampler::SetProperty( const int _SampleCount , const int _SampleGroupCount )
{
	SampleCount = _SampleCount;
	SampleGroupCount = _SampleGroupCount;

	SetupShuffledIndices();

	GenerateUnitSquareSamples();
}

void Sampler::SetupShuffledIndices()
{
	ShuffledIndices.reserve( SampleGroupCount * SampleCount );
	std::vector<int> Indices;

	for( int j = 0; j < SampleCount; j++ )
	{
		Indices.push_back( j );
	}

	// ShuffledIndices中存放了NumSets组采样点集
	// 每个采样点集中的值为0~NumSamples的一个随机排列
	for( int i = 0; i < SampleGroupCount; i++ )
	{
		random_shuffle( Indices.begin() , Indices.end() );

		for( int j = 0; j < SampleCount; j++ )
		{
			ShuffledIndices.push_back( Indices[j] );
		}
	}
}

void Sampler::ShuffleXCoordinate()
{
	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount - 1; j++ )
		{
			int target = i * SampleCount + rand() % SampleCount;

			double temp = SamplePoints[i * SampleCount + j + 1]->ImageSamples.x;
			SamplePoints[i * SampleCount + j + 1]->ImageSamples.x = SamplePoints[target]->ImageSamples.x;
			SamplePoints[target]->ImageSamples.x = temp;
		}
	}

	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount - 1; j++ )
		{
			int target = i * SampleCount + rand() % SampleCount;

			double temp = SamplePoints[i * SampleCount + j + 1]->LensSamples.x;
			SamplePoints[i * SampleCount + j + 1]->LensSamples.x = SamplePoints[target]->LensSamples.x;
			SamplePoints[target]->LensSamples.x = temp;
		}
	}
}

void Sampler::ShuffleYCoordinate()
{
	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount - 1; j++ )
		{
			int target = i * SampleCount + rand() % SampleCount;
			double temp = SamplePoints[i * SampleCount + j + 1]->ImageSamples.y;
			SamplePoints[i * SampleCount + j + 1]->ImageSamples.y = SamplePoints[target]->ImageSamples.y;
			SamplePoints[target]->ImageSamples.y = temp;
		}
	}

	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount - 1; j++ )
		{
			int target = i * SampleCount + rand() % SampleCount;

			double temp = SamplePoints[i * SampleCount + j + 1]->LensSamples.y;
			SamplePoints[i * SampleCount + j + 1]->LensSamples.y = SamplePoints[target]->LensSamples.y;
			SamplePoints[target]->LensSamples.y = temp;
		}
	}
}


CameraSample Sampler::GetSamplePoint()
{
	int Jump = 0;

	if( SamplePos % SampleCount == 0 )
	{
		Jump = ( rand() % SampleGroupCount ) * SampleCount;
	}

	SamplePos = ( SamplePos + 1 ) % SampleCount;

	return *SamplePoints[Jump + ShuffledIndices[Jump + SamplePos]];
}