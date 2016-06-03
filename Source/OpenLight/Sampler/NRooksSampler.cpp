#include "Utilities/PCH.h"
#include "tinyxml2.h"
#include "Utilities/RNG.h"
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
			// 沿着对角线采样
			CameraSample* samples = new CameraSample;
			samples->ImageSamples = Point2f( ( j + RNG::Get().GetDouble() ) / ( double )SampleCount , ( j + RNG::Get().GetDouble() ) / ( double )SampleCount );
			samples->LensSamples = Point2f( ( j + RNG::Get().GetDouble() ) / ( double )SampleCount , ( j + RNG::Get().GetDouble() ) / ( double )SampleCount );
			samples->TimeSamples = ( j + RNG::Get().GetDouble() ) / ( double )SampleCount;

			SamplePoints.push_back( samples );
		}
	}

	// x方向随机排列
	ShuffleXCoordinate();

	// y方向随机排列
	ShuffleYCoordinate();
}

void NRooksSampler::Deserialization( tinyxml2::XMLElement* SamplerRootElement )
{
	SamplerRootElement->QueryIntAttribute( "SampleGroupCount" , &SampleGroupCount );
	SamplerRootElement->QueryIntAttribute( "SampleCount" , &SampleCount );

	SetProperty( SampleCount , SampleGroupCount );
}

void NRooksSampler::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		pRootElement->SetAttribute( "SampleGroupCount" , SampleGroupCount );

		pRootElement->SetAttribute( "SampleCount" , SampleCount );
	}
}