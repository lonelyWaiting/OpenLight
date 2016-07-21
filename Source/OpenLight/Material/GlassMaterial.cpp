#include "Utilities/PCH.h"
#include "BRDF/SpecularReflection.h"
#include "BRDF/SpecularTransmission.h"
#include "BRDF/FresnelDielectric.h"
#include "tinyxml2.h"
#include "GlassMaterial.h"
#include "Utilities/srString.h"
#include "Texture/ConstantTexture.h"

GlassMaterial::GlassMaterial() : Material()
{
	/*Kr = new ConstantTexture( Spectrum( 1.0 ) );

	Kt = new ConstantTexture( Spectrum( 1.0 ) );*/
}

GlassMaterial::GlassMaterial( Spectrum R , Spectrum T , float ior )
	: Material()
{
	Kr = new ConstantTexture( R );

	Kt = new ConstantTexture( T );

	RefraIndex = new ConstantTexture( ior );
}

BSDF* GlassMaterial::GetBSDF( const Vector2f& uv , const Point3f& point , const Vector3f& normal ) const
{
	BSDF* bsdf = new BSDF();

	Spectrum R   = Kr->Evalute( uv , point );

	Spectrum T   = Kt->Evalute( uv , point );

	Spectrum ior = RefraIndex->Evalute( uv , point );

	bsdf->Add( new SpecularReflection( R , new FresnelDielectric( 1.0 , ior[0] ) ) );

	bsdf->Add( new SpecularTransmission( 1.0 , ior[0] , T ) );

	return bsdf;
}

void GlassMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
{
	tinyxml2::XMLElement* pKrElement = RootElement->FirstChildElement( "kr" );

	if( pKrElement )
	{
		Kr = Texture::Create( pKrElement->Attribute( "type" ) );

		Kr->Deserialization( pKrElement );
	}
	else
	{
		Kr = new ConstantTexture( Spectrum( 1.0 ) );
	}

	tinyxml2::XMLElement* pKtElement = RootElement->FirstChildElement( "kt" );

	if( pKtElement )
	{
		Kt = Texture::Create( pKtElement->Attribute( "type" ) );

		Kt->Deserialization( pKtElement );
	}
	else
	{
		Kt = new ConstantTexture( Spectrum( 1.0 ) );
	}

	tinyxml2::XMLElement* pRefraElement = RootElement->FirstChildElement( "RefraIndex" );

	if( pRefraElement )
	{
		RefraIndex = Texture::Create( pRefraElement->Attribute( "type" ) );

		RefraIndex->Deserialization( pRefraElement );
	}
	else
	{
		RefraIndex = new ConstantTexture( Spectrum( 1.5 ) );
	}
}

void GlassMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pKrElement = xmlDoc.NewElement( "kr" );

		Kr->Serialization( xmlDoc , pKrElement );

		pRootElement->InsertEndChild( pKrElement );
	}

	{
		tinyxml2::XMLElement* pKrElement = xmlDoc.NewElement( "kt" );

		Kt->Serialization( xmlDoc , pKrElement );

		pRootElement->InsertEndChild( pKrElement );
	}

	{
		tinyxml2::XMLElement* pKrElement = xmlDoc.NewElement( "RefraIndex" );

		RefraIndex->Serialization( xmlDoc , pKrElement );

		pRootElement->InsertEndChild( pKrElement );
	}
}

Texture* GlassMaterial::GetKr()
{
	return Kr;
}

Texture* GlassMaterial::GetKt()
{
	return Kt;
}

Texture* GlassMaterial::GetRefraIndex()
{
	return RefraIndex;
}