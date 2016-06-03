#pragma once

#include <map>

class ImageFile;

class ImageManager
{
public:
	ImageFile* GetImage( const char* filename );

	static ImageManager* GetInstance();

private:
	ImageManager();

private:
	std::map<const char* , ImageFile*> ImageList;

private:
	static ImageManager* m_pInstance;
};