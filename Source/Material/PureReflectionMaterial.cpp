#include "Utilities/PCH.h"
#include "BRDF/PureSpecularReflection.h"
#include "tinyxml2.h"
#include "PureReflectionMaterial.h"

PureReflectionMaterial::PureReflectionMaterial() : Material()
{

}

PureReflectionMaterial::PureReflectionMaterial( Spectrum R , double ior )
	: Material()
	, R( R )
	, ior( ior )
{

}

BSDF* PureReflectionMaterial::GetBSDF( const Point3f& point , const Normal& normal ) const
{
	BSDF* bsdf = new BSDF();

	bsdf->Add( new PureSpecularReflection( R ) );

	return bsdf;
}

void PureReflectionMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
{
	tinyxml2::XMLElement* pReflection = RootElement->FirstChildElement( "Reflection" );

	double r , g , b;
	pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
	pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
	pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

	R = Spectrum::FromRGB( r , g , b );
}