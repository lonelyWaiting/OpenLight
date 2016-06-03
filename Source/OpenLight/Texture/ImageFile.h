#pragma once

#include "Spectrum/Spectrum.h"
#include "FreeImage.h"

class ImageFile
{
public:
	ImageFile();

	ImageFile( std::wstring& filename );

	~ImageFile();

	bool LoadImage( std::wstring& filename );

	Spectrum Evalute( double u , double v );

private:
	std::wstring filename;

private:
	FIBITMAP* m_pDIB;
	BYTE* m_pBits;

private:
	int m_iWidth;
	int m_iHeight;
	int m_iPitch;
	int m_iBpp;
};