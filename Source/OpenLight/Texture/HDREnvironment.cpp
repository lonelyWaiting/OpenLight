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

Spectrum HDREnvironment::Evalute( Ray& ray )
{
	Vector3f Dir = ray.Direction;
	Dir = Normalize( Dir );

	double r = 0.159154943*acos( Dir[2] ) / sqrt( Dir[0] * Dir[0] + Dir[1] * Dir[1] );
	double u = 0.5 + Dir[0] * r;
	double v = 0.5 + Dir[1] * r;

	int Width , Height;
	m_pImage->GetSize( Width , Height );

	int col = ( int )( u * Width );
	int row = ( int )( ( 1 - v ) * Height );

	FIRGBF* pixels = ( FIRGBF* )FreeImage_GetBits( m_pImage->GetDataPtr() );

	return Spectrum::FromRGB( ( double )pixels[row*Height + col].red , ( double )pixels[row*Height + col].green , ( double )pixels[row*Height + col].blue );
}

void HDREnvironment::Deserialization( tinyxml2::XMLElement* RootElement )
{
	const char* pFilename = RootElement->Attribute( "filename" );
	m_Filename = new char[strlen( pFilename ) + 1];
	strcpy( m_Filename , pFilename );

	m_pImage = ImageManager::GetInstance()->GetImage( m_Filename );
}

void HDREnvironment::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "filename" , m_Filename );
}