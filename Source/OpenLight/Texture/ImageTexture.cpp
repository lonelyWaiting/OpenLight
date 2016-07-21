#include "ImageTexture.h"
#include "tinyxml2.h"
#include "ImageManager.h"
#include "ImageFile.h"
#include <string>

ImageTexture::ImageTexture()
{

}

ImageTexture::~ImageTexture()
{

}

Spectrum ImageTexture::Evalute( const Vector2f& uv , const Point3f& p ) const
{
	return m_pImage->Evalute( uv.x , uv.y );
}

void ImageTexture::Deserialization( tinyxml2::XMLElement* RootElement )
{
	const char* pFilename = RootElement->Attribute( "filename" );
	m_Filename = new char[strlen( pFilename ) + 1];
	strcpy( m_Filename , pFilename );

	m_pImage = ImageManager::GetInstance()->GetImage( m_Filename );
}

void ImageTexture::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute("type", GetName());

	pRootElement->SetAttribute("filename", m_Filename);
}

char* ImageTexture::GetFilename()
{
	return m_Filename;
}

void ImageTexture::SetFilename(char* pFilename)
{
	strcpy(m_Filename, pFilename);

	m_pImage = ImageManager::GetInstance()->GetImage(m_Filename);
}