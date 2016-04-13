#include "PCH.h"
#include "PureRandomSampler.h"

PureRandomSampler::PureRandomSampler()
	:Sampler()
{
	GenerateUnitSquareSamples();
}

PureRandomSampler::PureRandomSampler( const int num )
	: Sampler( num )
{
	GenerateUnitSquareSamples();
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

PureRandomSampler::~PureRandomSampler()
{

}

PureRandomSampler* PureRandomSampler::clone() const
{
	return ( new PureRandomSampler( *this ) );
}

void PureRandomSampler::GenerateUnitSquareSamples()
{
	for( int i = 0; i < NumSets; i++ )
	{
		for( int j = 0; j < NumSamples; j++ )
		{
			CameraSample* samples = new CameraSample;
			samples->ImageSamples = Point2f( ( double )rand() / RAND_MAX , ( double )rand() / RAND_MAX );
			samples->LensSamples  = Point2f( ( double )rand() / RAND_MAX , ( double )rand() / RAND_MAX );
			samples->TimeSamples  = ( double )rand() / RAND_MAX;

			SamplePoints.push_back( samples );
		}
	}
}