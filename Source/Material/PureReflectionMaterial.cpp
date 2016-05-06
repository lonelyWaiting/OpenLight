#include "PCH.h"
#include "BRDF/PureSpecularReflection.h"
#include "PureReflectionMaterial.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( PureReflectionMaterial , Material )

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

void PureReflectionMaterial::Deserialization( XMLElement* RootElement )
{
	XMLElement* pReflection = RootElement->FirstChildElement( "Reflection" );

	double r , g , b;
	pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
	pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
	pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

	R = Spectrum::FromRGB( r , g , b );
}