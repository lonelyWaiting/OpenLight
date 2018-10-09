#pragma once

#include "Spectrum/Spectrum.h"

// Read And Write Image
class Image
{
public:
	Image( char* filename , int width , int height );
	~Image();

	virtual void SetColor( int row , int col , Spectrum spectrum ) = 0;

	virtual Spectrum GetColor( int row , int col ) const = 0;

	virtual void Display() = 0;

	const void* GetData() { return data; }

protected:
	Image();

protected:
	char* Filename;
	int Width;
	int Height;
	Spectrum* data;
};