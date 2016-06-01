#include "Utilities/PCH.h"
#include "BRDF/PureSpecularReflection.h"
#include "tinyxml2.h"
#include "PureReflectionMaterial.h"

PureReflectionMaterial::PureReflectionMaterial() : Material()
{

}

PureReflectionMaterial::PureReflectionMaterial( Spectrum R )
	: Material()
	, R( R )
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

	if( pReflection )
	{
		double r , g , b;
		pReflection->FirstChildElement( "r" )->QueryDoubleText( &r );
		pReflection->FirstChildElement( "g" )->QueryDoubleText( &g );
		pReflection->FirstChildElement( "b" )->QueryDoubleText( &b );

		R = Spectrum::FromRGB( r , g , b );
	}
	else
	{
		R = Spectrum::FromRGB( 1.0 , 1.0 , 1.0 );
	}
}

void PureReflectionMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
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

Spectrum PureReflectionMaterial::GetReflection()
{
	return R;
}

void PureReflectionMaterial::SetReflection( float* reflection )
{
	R = Spectrum::FromRGB( ( double )reflection[0] , ( double )reflection[1] , ( double )reflection[2] );
}