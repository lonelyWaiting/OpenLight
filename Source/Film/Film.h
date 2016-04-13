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

	void SetGamma( double gamma );

	void SetResolution( int xResolution , int yResolution );

	void SetColor( int row , int col , Spectrum color );

	void Display();

public:
	Vector2f GetResolution() const;

	double GetAspectio() const;

public:
	void ParseFilm( const XMLElement* FilmRootElement );

private:
	int Width;
	int Height;

	double Gamma;
	double InvGamma;

	char* Filename;

	Image* image;
};