#include "PCH.h"
#include "Light/Light.h"

Light::~Light()
{

}

Light::Light( const Transform& _LightToWorld , int _nSamples )
	: LightToWorld( _LightToWorld )
	, WorldToLight( Inverse( _LightToWorld ) )
	, nSamples( _nSamples )
{

}

Spectrum Light::Le( const Ray& ray ) const
{
	return Spectrum( 0 );
}