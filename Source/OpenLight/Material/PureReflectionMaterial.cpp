#include "Utilities/PCH.h"
#include "BRDF/PureSpecularReflection.h"
#include "tinyxml2.h"
#include "PureReflectionMaterial.h"
#include "Utilities/srString.h"

PureReflectionMaterial::PureReflectionMaterial() : Material()
{
	R = Spectrum::FromRGB( 1.0 , 1.0 , 1.0 );
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
		ParseVector3( pReflection->GetText() , R.GetDataPtr() );
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
		char* pText = new char[27];
		sprintf( pText , "%f,%f,%f" , R[0] , R[1] , R[2] );

		tinyxml2::XMLElement* pReflectionElement = xmlDoc.NewElement( "Reflection" );

		pReflectionElement->SetText( pText );

		pRootElement->InsertEndChild( pReflectionElement );

		//SAFE_DELETE_ARRAY( pText );
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