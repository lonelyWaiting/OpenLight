#pragma once

#include "Image.h"

class PPMImage : public Image
{
public:
	PPMImage( char* filename , int width , int height );

public:
	virtual void SetColor( int row , int col , Spectrum spectrum );

	virtual Spectrum GetColor( int row , int col ) const;

	virtual void Display();

protected:
	PPMImage();

private:
	Spectrum* data;
};