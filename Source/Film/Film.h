#pragma once

#include "Spectrum/Spectrum.h"
#include "Image.h"
#include "Math/Vector2.h"

class Film
{
public:
	Film();

public:
	void SetFilename( char* filename );

	void SetGamma( float gamma );

	void SetResolution( float xResolution , float yResolution );

	void SetColor( int row , int col , Spectrum color );

	void Display();

public:
	Vector2f GetResolution() const;

	float GetAspectio() const;

private:
	float Width;
	float Height;

	float Gamma;
	float InvGamma;

	char* Filename;

	Image* image;
};