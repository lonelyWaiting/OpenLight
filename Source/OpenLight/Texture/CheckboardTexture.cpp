#include "CheckboardTexture.h"
#include "ConstantTexture.h"
#include "tinyxml2.h"
#include "Spectrum/Spectrum.h"
#include "Utilities/srString.h"

CheckboardTexture::CheckboardTexture()
{
	/*texture1 = new ConstantTexture( Spectrum( 0.0 ) );
	texture2 = new ConstantTexture( Spectrum( 1.0 ) );*/
}

CheckboardTexture::CheckboardTexture( ConstantTexture *tex1 , ConstantTexture* tex2 )
{
	texture1 = tex1;
	texture2 = tex2;
}

CheckboardTexture::~CheckboardTexture()
{
	SAFE_DELETE( texture1 );
	SAFE_DELETE( texture2 );
}

Spectrum CheckboardTexture::Evalute( const Vector2f& uv , const Point3f& p ) const
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

void CheckboardTexture::Deserialization( tinyxml2::XMLElement* RootElement )
{
	Spectrum value;

	ParseVector3( RootElement->Attribute( "value1" ) , value.GetDataPtr() );
	texture1 = new ConstantTexture( value );

	ParseVector3( RootElement->Attribute( "value2" ) , value.GetDataPtr() );
	texture2 = new ConstantTexture( value );
}

void CheckboardTexture::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
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

void CheckboardTexture::GetValue( Spectrum& value1 , Spectrum& value2 )
{
	value1 = texture1->GetValue();
	value2 = texture2->GetValue();
}

void CheckboardTexture::SetValue( Spectrum& value , int index )
{
	if( index == 1 )
	{
		texture1->SetValue( value );
	}
	else if( index == 2 )
	{
		texture2->SetValue( value );
	}
	else
	{
		assert( "index invalid , please input 1 or 2" );
	}
}