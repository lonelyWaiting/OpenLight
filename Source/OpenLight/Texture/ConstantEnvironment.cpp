#define _CRT_SECURE_NO_WARNINGS

#include "ConstantEnvironment.h"
#include "tinyxml2.h"
#include "Utilities/srString.h"

ConstantEnvironment::ConstantEnvironment()
{
	backgroundColor = Spectrum( 0.0 );
}

ConstantEnvironment::ConstantEnvironment( Spectrum& color )
	:backgroundColor( color )
{

}

Spectrum ConstantEnvironment::Evalute( Ray& r )
{
	return backgroundColor;
}

void ConstantEnvironment::Deserialization( tinyxml2::XMLElement* RootElement )
{
	ParseVector( RootElement->Attribute( "color" ) , backgroundColor.GetDataPtr() );
}

void ConstantEnvironment::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	char* pText = new char[50];
	sprintf( pText , "%f,%f,%f" , backgroundColor[0] , backgroundColor[1] , backgroundColor[2] );

	pRootElement->SetAttribute( "color" , pText );

	SAFE_DELETE( pText );
}

void ConstantEnvironment::SetBGColor( Spectrum& color )
{
	backgroundColor = color;
}