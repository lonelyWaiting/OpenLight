#include "PCH.h"
#include "Spectrum/Spectrum.h"
#include "Image.h"

Image::Image( char* filename , int width , int height )
{
	Filename = filename;
	Width = width;
	Height = height;
}