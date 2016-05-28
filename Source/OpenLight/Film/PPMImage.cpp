#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "PPMImage.h"

PPMImage::PPMImage( char* filename , int width , int height )
	:Image( filename , width , height )
{
	data = new Spectrum[Width * height];
}

PPMImage::~PPMImage()
{
	SAFE_DELETE_ARRAY( data );
}

void PPMImage::SetColor( int row , int col , Spectrum spectrum )
{
	if( 0 > col || col >= Width )
	{
		Log::Get().Error( "col value is outside of Image resolution in %s , %s line" , __FILE__ , __LINE__ );
	}

	if( 0 > row || row > Height )
	{
		Log::Get().Error( "row value is outside of Image resolution in %s , %s line" , __FILE__ , __LINE__ );
	}

	data[row * Width + col] = spectrum;

	GammaCorrect( row , col );
}

Spectrum PPMImage::GetColor( int row , int col ) const
{
	if( 0 > col || col >= Width )
	{
		Log::Get().Error( "col value is outside of Image resolution in %s , %s line" , __FILE__ , __LINE__ );
	}

	if( 0 > row || row > Height )
	{
		Log::Get().Error( "row value is outside of Image resolution in %s , %s line" , __FILE__ , __LINE__ );
	}

	return data[row * Width + col];
}

void PPMImage::Display()
{
	std::ofstream ofs( Filename , std::ios::out | std::ios::binary );

	ofs << "P6\n";
	ofs << Width << ' ' << Height << '\n';
	ofs << "255\n";

	unsigned int ired , igreen , iblue;
	unsigned char red , green , blue;

	double RGB[3];

	clock_t start = clock();

	for( int row = 0; row < Height; row++ )
	{
		fprintf( stdout , "\rWriting Image Data: %8.2f%%" , ( double )row / ( double )( Height - 1 ) *100 );

		for( int col = 0; col < Width; col++ )
		{
			data[row * Width + col].ToRGB( RGB );

			ired   = ( unsigned int )( 255 * RGB[0] );
			igreen = ( unsigned int )( 255 * RGB[1] );
			iblue  = ( unsigned int )( 255 * RGB[2] );

			Clamp( ired   , 1 , 255 );
			Clamp( igreen , 1 , 255 );
			Clamp( iblue  , 1 , 255 );

			red   = ( unsigned char )( ired );
			green = ( unsigned char )( igreen );
			blue  = ( unsigned char )( iblue );

			ofs << red << green << blue;
		}
	}

	clock_t end = clock();
	double t = ( double )( end - start ) / CLOCKS_PER_SEC;
	printf( "\nWriting time: %fs.\n" , t );
	Log::Get().Info( "\nWriting time: %fs.\n" , t );
}

void PPMImage::GammaCorrect( int row , int col )
{ 
	for( int i = 0; i < 3; i++ )
	{
		data[row * Width + col][i] = std::pow( data[row * Width + col][i] , 1.0 / 2.2 );
	}
}