#include "Utilities/PCH.h"
#include "BRDF/Lambertian.h"
#include "tinyxml2.h"
#include "DiffuseMaterial.h"
#include "Utilities/srString.h"
#include "Texture/ConstantTexture.h"

DiffuseMaterial::DiffuseMaterial() : Material()
{
	/*Kd = new ConstantTexture( Spectrum( 1.0 ) );*/
}

DiffuseMaterial::DiffuseMaterial( Texture* kd )
	: Material()
	, Kd( kd )
{

}

void DiffuseMaterial::SetKd( Texture* kd )
{
	Kd = kd;
}

BSDF* DiffuseMaterial::GetBSDF( const Vector2f& uv , const Point3f& point , const Normal& normal ) const
{
	BSDF* bsdf = new BSDF();

	// 计算出对应位置的纹理值
	Spectrum R = Kd->Evalute( uv , point );

	bsdf->Add( new Lambertian( R ) );

	return bsdf;
}

void DiffuseMaterial::Deserialization( tinyxml2::XMLElement* RootElement )
{
	tinyxml2::XMLElement* pKdElement = RootElement->FirstChildElement( "kd" );

	if( pKdElement )
	{
		Kd = Texture::Create( pKdElement->Attribute( "type" ) );

		Kd->Deserialization( pKdElement );
	}
	else
	{
		Kd = new ConstantTexture( Spectrum( 1.0 ) );
	}
}

void DiffuseMaterial::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pKdElement = xmlDoc.NewElement( "kd" );

		Kd->Serialization( xmlDoc , pKdElement );

		pRootElement->InsertEndChild( pKdElement );
	}
}

Texture* DiffuseMaterial::GetKd()
{
	return Kd;
}