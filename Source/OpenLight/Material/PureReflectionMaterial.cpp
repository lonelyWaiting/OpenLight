#include "Utilities/PCH.h"
#include "BRDF/PureSpecularReflection.h"
#include "tinyxml2.h"
#include "PureReflectionMaterial.h"
#include "Utilities/srString.h"
#include "Texture/ConstantTexture.h"

PureReflectionMaterial::PureReflectionMaterial() : Material()
{
	/*Kr = new ConstantTexture( Spectrum( 1.0 ) );*/
}

PureReflectionMaterial::PureReflectionMaterial( Spectrum R )
	: Material()
{
	Kr = new ConstantTexture( R );
}

BSDF* PureReflectionMaterial::GetBSDF( const Vector2f& uv , const Point3f& point , const Normal& normal ) const
{
	BSDF* bsdf = new BSDF();

	Spectrum R = Kr->Evalute( uv , point );

	bsdf->Add( new PureSpecularReflection( R ) );

	return bsdf;
}

void PureReflectionMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
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
}

void PureReflectionMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pKrElement = xmlDoc.NewElement( "kr" );

		Kr->Serialization( xmlDoc , pKrElement );

		pRootElement->InsertEndChild( pKrElement );
	}
}

Texture* PureReflectionMaterial::Getkr()
{
	return Kr;
}