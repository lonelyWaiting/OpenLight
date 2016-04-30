#include "PCH.h"
#include "BRDF/Lambertian.h"
#include "DiffuseMaterial.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( DiffuseMaterial , Material )

DiffuseMaterial::DiffuseMaterial() : Material()
{

}

DiffuseMaterial::DiffuseMaterial( Spectrum R )
	: Material()
	, R( R )
{

}

BSDF* DiffuseMaterial::GetBSDF( const Point3f& point , const Normal& normal ) const
{
	BSDF* bsdf = new BSDF();

	double ReflectRGB[3] = { 1.0 , 1.0 , 1.0 };
	R = Spectrum::FromRGB( ReflectRGB );

	bsdf->Add( new Lambertian( R ) );

	return bsdf;
}

void DiffuseMaterial::Deserialization( XMLElement* RootElement )
{
	XMLElement* pReflection = RootElement->FirstChildElement( "Reflection" );

	double r , g , b;
	pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
	pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
	pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

	R = Spectrum::FromRGB( r , g , b );
}