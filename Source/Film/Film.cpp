#include "PCH.h"
#include "PPMImage.h"
#include "Film.h"

Film::Film()
{
	Width = 800;
	Height = 600;

	Gamma = 2.4;
	InvGamma = 1.0f / Gamma;

	Filename = "result.ppm";

	image = new PPMImage( Filename , Width , Height );
}

void Film::SetResolution( int xResolution , int yResolution )
{
	Width = xResolution;
	Height = yResolution;
}

void Film::SetGamma( double gamma )
{
	Gamma = gamma;
	InvGamma = 1.0f / Gamma;
}

void Film::SetFilename( char* filename )
{
	Filename = filename;

	std::string name( Filename );

	if( name.size() >= 5 )
	{
		uint32_t suffixOffset = name.size() - 4;
		
		if( !strcmp( name.c_str() + suffixOffset , ".ppm" ) ||
			!strcmp( name.c_str() + suffixOffset , ".PPM" ) )
		{
			if( image )
			{
				SAFE_DELETE( image );
			}

			image = new PPMImage( Filename , Width , Height );
		}
	}
}

void Film::SetColor( int row , int col , Spectrum color )
{
	image->SetColor( row , col , color );
}

double Film::GetAspectio() const
{
	return double( Width ) / double( Height );
}

void Film::Display()
{
	image->Display();
}

Vector2f Film::GetResolution() const
{
	return Vector2f( Width , Height );
}

void Film::ParseFilm( const XMLElement* FilmRootElement )
{
	// ---------------------------------Film---------------------------------------------
	Filename = new char[20];
	strcpy_s( Filename , 20 , FilmRootElement->FirstChildElement( "filename" )->GetText() );

	FilmRootElement->FirstChildElement( "width" )->QueryIntText( &Width );
	FilmRootElement->FirstChildElement( "height" )->QueryIntText( &Height );

	SAFE_DELETE( image );

	image = new PPMImage( Filename , Width , Height );
}