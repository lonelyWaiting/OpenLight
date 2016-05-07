#pragma once

#include "Spectrum/Spectrum.h"
#include "Image.h"
#include "Math/Vector2.h"

namespace tinyxml2
{
	class XMLElement;
}

class Film
{
public:
	Film();

public:
	void SetFilename( char* filename );

	void SetGamma( double gamma );

	void SetResolution( int xResolution , int yResolution );

	void SetColor( int row , int col , Spectrum color );

	void Display();

	Image* GetImage();

public:
	Vector2f GetResolution() const;

	double GetAspectio() const;

public:
	void Deserialization( const tinyxml2::XMLElement* FilmRootElement );

private:
	int Width;
	int Height;

	double Gamma;
	double InvGamma;

	char* Filename;

	Image* image;
};