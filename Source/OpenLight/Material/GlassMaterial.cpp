#include "Utilities/PCH.h"
#include "BRDF/SpecularReflection.h"
#include "BRDF/SpecularTransmission.h"
#include "BRDF/FresnelDielectric.h"
#include "tinyxml2.h"
#include "GlassMaterial.h"
#include "Utilities/srString.h"

GlassMaterial::GlassMaterial() : Material()
{
	R = Spectrum::FromRGB( 1.0 , 1.0 , 1.0 );

	T = Spectrum::FromRGB( 1.0 , 1.0 , 1.0 );
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

	if( pReflection )
	{
		ParseVector3( pReflection->GetText() , R.GetDataPtr() );
	}
	else
	{
		R = Spectrum::FromRGB( 1.0 , 1.0 , 1.0 );
	}

	tinyxml2::XMLElement* pTransmission = RootElement->FirstChildElement( "Transmission" );

	if( pTransmission )
	{
		ParseVector3( pTransmission->GetText() , T.GetDataPtr() );
	}
	else
	{
		R = Spectrum::FromRGB( 1.0 , 1.0 , 1.0 );
	}

	RootElement->FirstChildElement( "ior" )->QueryDoubleText( &ior );
}

void GlassMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		char* pText = new char[27];
		sprintf( pText , "%f,%f,%f" , R[0] , R[1] , R[2] );

		tinyxml2::XMLElement* pReflectionElement = xmlDoc.NewElement( "Reflection" );

		pReflectionElement->SetText( pText );

		pRootElement->InsertEndChild( pReflectionElement );

		//SAFE_DELETE_ARRAY( pText );
	}

	{
		char* pText = new char[27];
		sprintf( pText , "%f,%f,%f" , T[0] , T[1] , T[2] );

		tinyxml2::XMLElement* pTransmissionElement = xmlDoc.NewElement( "Transmission" );

		pTransmissionElement->SetText( pText );

		pRootElement->InsertEndChild( pTransmissionElement );

		//SAFE_DELETE_ARRAY( pText );
	}

	{
		tinyxml2::XMLElement* pIORElement = xmlDoc.NewElement( "ior" );

		pIORElement->SetText( ior );

		pRootElement->InsertEndChild( pIORElement );
	}
}

Spectrum GlassMaterial::GetReflection()
{
	return R;
}

Spectrum GlassMaterial::GetTransmission()
{
	return T;
}

double GlassMaterial::GetIOR()
{
	return ior;
}

void GlassMaterial::SetReflection( float* r )
{
	R = Spectrum::FromRGB( r[0] , r[1] , r[2] );
}

void GlassMaterial::SetTransmission( float* t )
{
	T = Spectrum::FromRGB( t[0] , t[1] , t[2] );
}

void GlassMaterial::SetIOR( float _ior )
{
	ior = _ior;
}