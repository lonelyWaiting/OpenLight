#include "PCH.h"
#include "NRooksSampler.h"

NRooksSampler::NRooksSampler()
	:Sampler()
{
	
}

NRooksSampler::NRooksSampler( const NRooksSampler& rhs )
	: Sampler( rhs )
{
	GenerateUnitSquareSamples();
}

NRooksSampler& NRooksSampler::operator = ( const NRooksSampler& rhs )
{
	if( this == &rhs )
	{
		return ( *this );
	}

	Sampler::operator=( rhs );

	return ( *this );
}

NRooksSampler::~NRooksSampler()
{

}

void NRooksSampler::GenerateUnitSquareSamples()
{
	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount; j++ )
		{
			CameraSample* samples = new CameraSample;
			samples->ImageSamples = Point2f( ( double )rand() / RAND_MAX , ( double )rand() / RAND_MAX );
			samples->LensSamples = Point2f( ( double )rand() / RAND_MAX , ( double )rand() / RAND_MAX );
			samples->TimeSamples = ( double )rand() / RAND_MAX;

			SamplePoints.push_back( samples );
		}
	}

	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount; j++ )
		{
			// 沿着对角线采样
			CameraSample* samples = new CameraSample;
			samples->ImageSamples = Point2f( ( j + ( double )rand() ) / ( double )SampleCount , ( j + ( double )rand() ) / ( double )SampleCount );
			samples->LensSamples = Point2f( ( j + ( double )rand() ) / ( double )SampleCount , ( j + ( double )rand() ) / ( double )SampleCount );
			samples->TimeSamples = ( j + ( double )rand() ) / ( double )SampleCount;

			SamplePoints.push_back( samples );
		}
	}

	// x方向随机排列
	ShuffleXCoordinate();

	// y方向随机排列
	ShuffleYCoordinate();
}

void NRooksSampler::Deserialization( XMLElement* SamplerRootElement )
{
	SamplerRootElement->FirstChildElement( "SampleGroupCount" )->QueryIntText( &SampleGroupCount );
	SamplerRootElement->FirstChildElement( "SampleCount" )->QueryIntText( &SampleCount );

	SetProperty( SampleCount , SampleGroupCount );
}