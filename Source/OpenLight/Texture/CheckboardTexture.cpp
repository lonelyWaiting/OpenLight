#include "CheckboardTexture.h"
#include "ConstantTexture.h"
#include "tinyxml2.h"
#include "Spectrum/Spectrum.h"
#include "Utilities/srString.h"

CheckerboardTexture::CheckerboardTexture()
{
	/*texture1 = new ConstantTexture( Spectrum( 0.0 ) );
	texture2 = new ConstantTexture( Spectrum( 1.0 ) );*/
}

CheckerboardTexture::CheckerboardTexture( ConstantTexture *tex1 , ConstantTexture* tex2 )
{
	texture1 = tex1;
	texture2 = tex2;
}

CheckerboardTexture::~CheckerboardTexture()
{
	SAFE_DELETE( texture1 );
	SAFE_DELETE( texture2 );
}

Spectrum CheckerboardTexture::Evalute( const Vector2f& uv , const Point3f& p ) const
{
	if( ( ( int )uv.x + ( int )uv.y ) % 2 == 0 )
	{
		return texture1->Evalute( uv , p );
	}
	else
	{
		return texture2->Evalute( uv , p );
	}
}

void CheckerboardTexture::Deserialization( tinyxml2::XMLElement* RootElement )
{
	Spectrum value;

	ParseVector3( RootElement->Attribute( "value1" ) , value.GetDataPtr() );
	texture1 = new ConstantTexture( value );

	ParseVector3( RootElement->Attribute( "value2" ) , value.GetDataPtr() );
	texture2 = new ConstantTexture( value );
}

void CheckerboardTexture::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	{
		Spectrum& value = texture1->GetValue();

		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , value[0] , value[1] , value[2] );

		pRootElement->SetAttribute( "value1" , pText );
	}

	{
		Spectrum& value = texture2->GetValue();

		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , value[0] , value[1] , value[2] );

		pRootElement->SetAttribute( "value2" , pText );
	}
}