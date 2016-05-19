#include "Utilities/PCH.h"
#include "tinyxml2.h"
#include "Utilities/RNG.h"
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
			samples->ImageSamples = Point2f( RNG::Get().GetDouble() , RNG::Get().GetDouble() );
			samples->LensSamples  = Point2f( RNG::Get().GetDouble() , RNG::Get().GetDouble() );
			samples->TimeSamples  = RNG::Get().GetDouble();

			SamplePoints.push_back( samples );
		}
	}
}

void PureRandomSampler::Deserialization( tinyxml2::XMLElement* SamplerRootElement )
{
	SamplerRootElement->FirstChildElement( "SampleGroupCount" )->QueryIntText( &SampleGroupCount );
	SamplerRootElement->FirstChildElement( "SampleCount" )->QueryIntText( &SampleCount );

	SetProperty( SampleCount , SampleGroupCount );
}

void PureRandomSampler::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pSampleGroupCountElement = xmlDoc.NewElement( "SampleGroupCount" );

		pSampleGroupCountElement->SetText( SampleGroupCount );

		pRootElement->InsertEndChild( pSampleGroupCountElement );

		tinyxml2::XMLElement* pSampleCountElement = xmlDoc.NewElement( "SampleCount" );

		pSampleCountElement->SetText( SampleCount );

		pRootElement->InsertEndChild( pSampleCountElement );
	}
}