#include "PCH.h"
#include "BRDF/SpecularReflection.h"
#include "BRDF/SpecularTransmission.h"
#include "BRDF/FresnelDielectric.h"
#include "GlassMaterial.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( GlassMaterial , Material )

GlassMaterial::GlassMaterial() : Material()
{

}

GlassMaterial::GlassMaterial( Spectrum R , Spectrum T , double ior )
	: Material()
	, R( R )
	, T( T )
	, ior( ior )
{

}

BSDF* GlassMaterial::GetBSDF( const Point3f& point , const Normal& normal ) const
{
	BSDF* bsdf = new BSDF();

	/*double ReflectRGB[3] = { 1.0 , 1.0 , 1.0 };
	R = Spectrum::FromRGB( ReflectRGB );

	double Transmission[3] = { 1.0 , 1.0  ,1.0 };
	T = Spectrum::FromRGB( Transmission );*/

	bsdf->Add( new SpecularReflection( R , new FresnelDielectric( 1.0 , ior ) ) );

	bsdf->Add( new SpecularTransmission( 1.0 , ior , T ) );

	return bsdf;
}

void GlassMaterial::Deserialization( XMLElement* RootElement )
{
	XMLElement* pReflection = RootElement->FirstChildElement( "Reflection" );

	double r , g , b;
	pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
	pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
	pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

	R = Spectrum::FromRGB( r , g , b );

	XMLElement* pTransmission = RootElement->FirstChildElement( "Transmission" );

	pTransmission->FirstChildElement( "r" )->QueryDoubleText( &r );
	pTransmission->FirstChildElement( "g" )->QueryDoubleText( &g );
	pTransmission->FirstChildElement( "b" )->QueryDoubleText( &b );

	T = Spectrum::FromRGB( r , g , b );

	RootElement->FirstChildElement( "ior" )->QueryDoubleText( &ior );
}