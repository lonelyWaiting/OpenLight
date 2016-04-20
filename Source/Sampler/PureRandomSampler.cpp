#include "PCH.h"
#include "PureRandomSampler.h"

PureRandomSampler::PureRandomSampler()
	:Sampler()
{

}

PureRandomSampler::PureRandomSampler( const PureRandomSampler& r )
	: Sampler( r )
{
	GenerateUnitSquareSamples();
}

PureRandomSampler& PureRandomSampler::operator= ( const PureRandomSampler& rhs )
{
	if( this == &rhs )
	{
		return ( *this );
	}

	Sampler::operator=( rhs );

	return ( *this );
}

void PureRandomSampler::GenerateUnitSquareSamples()
{
	for( int i = 0; i < SampleGroupCount; i++ )
	{
		for( int j = 0; j < SampleCount; j++ )
		{
			CameraSample* samples = new CameraSample;
			samples->ImageSamples = Point2f( ( double )rand() / RAND_MAX , ( double )rand() / RAND_MAX );
			samples->LensSamples  = Point2f( ( double )rand() / RAND_MAX , ( double )rand() / RAND_MAX );
			samples->TimeSamples  = ( double )rand() / RAND_MAX;

			SamplePoints.push_back( samples );
		}
	}
}

void PureRandomSampler::Deserialization( XMLElement* SamplerRootElement )
{
	SamplerRootElement->FirstChildElement( "SampleGroupCount" )->QueryIntText( &SampleGroupCount );
	SamplerRootElement->FirstChildElement( "SampleCount" )->QueryIntText( &SampleCount );

	SetProperty( SampleCount , SampleGroupCount );
}