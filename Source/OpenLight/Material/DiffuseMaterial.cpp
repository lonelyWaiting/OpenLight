#include "Utilities/PCH.h"
#include "BRDF/Lambertian.h"
#include "tinyxml2.h"
#include "DiffuseMaterial.h"
#include "Utilities/srString.h"

DiffuseMaterial::DiffuseMaterial() : Material()
{
	R = Spectrum::FromRGB( 1 , 1 , 1 );
}

DiffuseMaterial::DiffuseMaterial( Spectrum R )
	: Material()
	, R( R )
{

}

BSDF* DiffuseMaterial::GetBSDF( const Point3f& point , const Normal& normal ) const
{
	BSDF* bsdf = new BSDF();

	bsdf->Add( new Lambertian( R ) );

	return bsdf;
}

void DiffuseMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
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
}

void DiffuseMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
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
}

Spectrum DiffuseMaterial::GetReflection()
{
	return R;
}

void DiffuseMaterial::SetReflection( float* r )
{
	R = Spectrum::FromRGB( r[0] , r[1] , r[2] );
}