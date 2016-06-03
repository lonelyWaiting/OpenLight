#include "ImageManager.h"
#include "ImageFile.h"
#include "Utilities/srString.h"

ImageManager* ImageManager::m_pInstance = nullptr;

ImageManager::ImageManager()
{

}

ImageManager* ImageManager::GetInstance()
{
	if( m_pInstance == nullptr )
	{
		m_pInstance = new ImageManager;
	}

	return m_pInstance;
}

ImageFile* ImageManager::GetImage( const char* filename )
{
	ImageFile* pImage = ImageList[filename];

	if( pImage == nullptr )

	{
		ImageList[filename] = new ImageFile( srString::ToUnicode( std::string( filename ) ) );
	}

	return ImageList[filename];
}