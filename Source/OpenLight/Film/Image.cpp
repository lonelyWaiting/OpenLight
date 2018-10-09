#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "Image.h"

Image::Image( char* filename , int width , int height )
{
	Filename = filename;
	Width    = width;
	Height   = height;
	data     = new Spectrum[Width * height];
	memset(data, 0, sizeof(Spectrum) * width * height);
}

Image::~Image()
{
	SAFE_DELETE_ARRAY(data);
}