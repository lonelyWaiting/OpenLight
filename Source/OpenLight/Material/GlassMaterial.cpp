#include "Utilities/PCH.h"
#include "BRDF/SpecularReflection.h"
#include "BRDF/SpecularTransmission.h"
#include "BRDF/FresnelDielectric.h"
#include "tinyxml2.h"
#include "GlassMaterial.h"

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

	bsdf->Add( new SpecularReflection( R , new FresnelDielectric( 1.0 , ior ) ) );

	bsdf->Add( new SpecularTransmission( 1.0 , ior , T ) );

	return bsdf;
}

void GlassMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
{
	tinyxml2::XMLElement* pReflection = RootElement->FirstChildElement( "Reflection" );

	double r , g , b;
	pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
	pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
	pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

	R = Spectrum::FromRGB( r , g , b );

	tinyxml2::XMLElement* pTransmission = RootElement->FirstChildElement( "Transmission" );

	pTransmission->FirstChildElement( "r" )->QueryDoubleText( &r );
	pTransmission->FirstChildElement( "g" )->QueryDoubleText( &g );
	pTransmission->FirstChildElement( "b" )->QueryDoubleText( &b );

	T = Spectrum::FromRGB( r , g , b );

	RootElement->FirstChildElement( "ior" )->QueryDoubleText( &ior );
}

void GlassMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
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

	{
		tinyxml2::XMLElement* pTransmissionElement = xmlDoc.NewElement( "Transmission" );

		pRootElement->InsertEndChild( pTransmissionElement );

		tinyxml2::XMLElement* pRElement = xmlDoc.NewElement( "r" );

		pRElement->SetText( T[0] );

		pTransmissionElement->InsertEndChild( pRElement );

		tinyxml2::XMLElement* pGElement = xmlDoc.NewElement( "g" );

		pGElement->SetText( T[1] );

		pTransmissionElement->InsertEndChild( pGElement );

		tinyxml2::XMLElement* pBElement = xmlDoc.NewElement( "b" );

		pBElement->SetText( T[2] );

		pTransmissionElement->InsertEndChild( pBElement );
	}

	{
		tinyxml2::XMLElement* pIORElement = xmlDoc.NewElement( "ior" );

		pIORElement->SetText( ior );

		pRootElement->InsertEndChild( pIORElement );
	}
}