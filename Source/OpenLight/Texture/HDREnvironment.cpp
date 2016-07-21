#define _CRT_SECURE_NO_WARNINGS

#include "HDREnvironment.h"
#include "tinyxml2.h"
#include "ImageManager.h"
#include "Math/Ray.h"
#include "ImageFile.h"

HDREnvironment::HDREnvironment()
{

}

HDREnvironment::~HDREnvironment()
{

}

Spectrum HDREnvironment::Evalute( Rayf& ray )
{
	Vector3f Dir = ray.Direction;
	Dir = Normalize( Dir );

	float r = 0.159154943*acos( Dir[2] ) / sqrt( Dir[0] * Dir[0] + Dir[1] * Dir[1] );
	float u = 0.5 + -Dir[0] * r;
	float v = 0.5 + Dir[1] * r;

	int Width , Height;
	m_pImage->GetSize( Width , Height );

	int col = ( int )( u * Width );
	int row = ( int )( ( 1 - v ) * Height );

	FIRGBF* pixels = ( FIRGBF* )FreeImage_GetBits( m_pImage->GetDataPtr() );

	return Spectrum::FromRGB( ( float )pixels[row*Height + col].red , ( float )pixels[row*Height + col].green , ( float )pixels[row*Height + col].blue );
}

void HDREnvironment::Deserialization( tinyxml2::XMLElement* pRootElement )
{
	const char* pFilename = pRootElement->Attribute( "filename" );
	m_Filename = new char[strlen( pFilename ) + 1];
	strcpy( m_Filename , pFilename );

	m_pImage = ImageManager::GetInstance()->GetImage( m_Filename );
}

void HDREnvironment::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute("type", GetName());

	pRootElement->SetAttribute( "filename" , m_Filename );
}