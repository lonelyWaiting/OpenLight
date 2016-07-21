#include "Utilities/PCH.h"
#include "Light/Light.h"

Light::~Light()
{

}

Light::Light()
{

}

Light::Light( const Transform& _LightToWorld , int _nSamples )
	: LightToWorld( _LightToWorld )
	, WorldToLight( Inverse( _LightToWorld ) )
	, nSamples( _nSamples )
{

}

Spectrum Light::Le( const Point3f& p , const Vector3f& n , const Vector3f& wo ) const
{
	return Spectrum( 0 );
}

bool Light::IsDelta()
{
	return false;
}
