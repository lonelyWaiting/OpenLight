#include "Utilities/PCH.h"
#include "BRDF/Lambertian.h"
#include "tinyxml2.h"
#include "DiffuseMaterial.h"

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

void DiffuseMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
{
	tinyxml2::XMLElement* pReflection = RootElement->FirstChildElement( "Reflection" );

	double r , g , b;
	pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
	pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
	pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

	R = Spectrum::FromRGB( r , g , b );
}

void DiffuseMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pReflectionElement = xmlDoc.NewElement( "Reflection" );

		pRootElement->InsertEndChild( pReflectionElement );

		tinyxml2::XMLElement* pRElement = xmlDoc.NewElement( "r" );

		pRElement->SetText( R[0] );

		pReflectionElement->InsertEndChild( pRElement );

		tinyxml2::XMLElement* pGElement = xmlDoc.NewElement( "g" );

		pGElement->SetText( R[1] );

		pReflectionElement->InsertEndChild( pGElement );

		tinyxml2::XMLElement* pBElement = xmlDoc.NewElement( "b" );

		pBElement->SetText( R[2] );

		pReflectionElement->InsertEndChild( pBElement );
	}
}