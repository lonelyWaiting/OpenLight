#include "ConstantTexture.h"
#include "Utilities/srString.h"
#include "tinyxml2.h"
#include "Spectrum/Spectrum.h"

ConstantTexture::ConstantTexture()
{
	value = Spectrum( 1.0 );
}

ConstantTexture::ConstantTexture( const Spectrum& v )
{
	value = v;
}

Spectrum ConstantTexture::Evalute( const Vector2f& uv , const Point3f& p ) const
{
	return value;
}

void ConstantTexture::Deserialization( tinyxml2::XMLElement* RootElement )
{
	ParseVector3( RootElement->Attribute( "value" ) , value.GetDataPtr() );
}

void ConstantTexture::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	{
		pRootElement->SetAttribute( "type" , GetName() );
	}

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , value[0] , value[1] , value[2] );

		pRootElement->SetAttribute( "value" , pText );

		SAFE_DELETE( pText );
	}
}

Spectrum& ConstantTexture::GetValue()
{ 
	return value;
}

void ConstantTexture::SetValue( Spectrum& _value )
{
	value = _value;
}