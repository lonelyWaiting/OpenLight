#include "ImageFile.h"
#include "Utilities/srString.h"
#include "IO/FileSystem.h"
#include "IO/Log.h"
#include "Spectrum/Spectrum.h"

ImageFile::ImageFile()
{

}

ImageFile::~ImageFile()
{
	if( m_pDIB != NULL )
	{
		FreeImage_Unload( m_pDIB );
	}
}

ImageFile::ImageFile( std::wstring& filename )
{
	LoadImage( filename );
}

bool ImageFile::LoadImage( std::wstring& filename )
{
	FileSystem fs;
	std::wstring Path = fs.GetTextureFolder() + filename;

	std::wstring ErrorText = L"Error Loading File" + filename + L" -> ";

	FREE_IMAGE_FORMAT FIF = FreeImage_GetFileType( srString::ToAscii( Path ).c_str() );

	if( FIF == FIF_UNKNOWN )
	{
		FIF = FreeImage_GetFIFFromFilename( srString::ToAscii( Path ).c_str() );
	}

	if( FIF == FIF_UNKNOWN )
	{
		Log::Get().Error( ErrorText + L"FIF is FIF_UNKNOWN" );
		return false;
	}

	if( FreeImage_FIFSupportsReading( FIF ) )
	{
		m_pDIB = FreeImage_Load( FIF , srString::ToAscii( Path ).c_str() );
	}

	if( m_pDIB == NULL )
	{
		Log::Get().Error( ErrorText + L"DIB is NULL" );
		return false;
	}

	m_iWidth = FreeImage_GetWidth( m_pDIB );
	m_iHeight = FreeImage_GetHeight( m_pDIB );
	m_iPitch = FreeImage_GetPitch( m_pDIB );
	m_iBpp = FreeImage_GetBPP( m_pDIB );

	if( m_iWidth == 0 || m_iHeight == 0 )
	{
		Log::Get().Error( ErrorText + L"Width or Height is 0" );
		return false;
	}

	if( m_iBpp != 24 && m_iBpp != 32 )
	{
		Log::Get().Error( ErrorText + L"BPP is not 24 or 32" );
		return false;
	}

	m_iBpp /= 8;

	m_pBits = FreeImage_GetBits( m_pDIB );

	if( m_pBits == nullptr )
	{
		Log::Get().Error( ErrorText + L"Bits is NULL" );
		return false;
	}

	return true;
}

Spectrum ImageFile::Evalute( float u , float v )
{
	if( m_pBits != nullptr )
	{
		u -= ( int )u;
		v -= ( int )v;

		if( u < 0.0 )	u += 1.0;
		if( v < 0.0 )	v += 1.0;

		int x = ( int )( u * ( m_iWidth - 1 ) );
		int y = ( int )( v * ( m_iHeight - 1 ) );

		int xnext = (int)clamp( (float)(x + 1) , 0.0f , (float)(m_iWidth - 1) );
		int ynext = (int)clamp( (float)(y + 1) , 0.0f , (float)(m_iHeight - 1) );

		u = x == xnext ? 0.0f : u;
		v = y == ynext ? 0.0f : v;

		BYTE* A = m_pBits + m_iPitch * ( m_iHeight - 1 - y ) + m_iBpp * x;
		BYTE* B = m_pBits + m_iPitch * ( m_iHeight - 1 - y ) + m_iBpp * xnext;
		BYTE* C = m_pBits + m_iPitch * ( m_iHeight - 1 - ynext ) + m_iBpp * x;
		BYTE* D = m_pBits + m_iPitch * ( m_iHeight - 1 - ynext ) + m_iBpp * xnext;
		
		float r = ( ( float )A[2] * ( 1.0f - u ) + ( float )B[2] * u ) * ( 1.0f - v ) + ( ( float )C[2] * ( 1.0f - u ) + ( float )D[2] * u ) * v;
		float g = ( ( float )A[1] * ( 1.0f - u ) + ( float )B[1] * u ) * ( 1.0f - v ) + ( ( float )C[1] * ( 1.0f - u ) + ( float )D[1] * u ) * v;
		float b = ( ( float )A[0] * ( 1.0f - u ) + ( float )B[0] * u ) * ( 1.0f - v ) + ( ( float )C[0] * ( 1.0f - u ) + ( float )D[0] * u ) * v;
		float inv = 1.0f / 255.0f;

		return Spectrum::FromRGB( r * inv , g * inv , b * inv );
	}

	return Spectrum::FromRGB( 0.0f , 0.0f , 0.0f );
}

FIBITMAP* ImageFile::GetDataPtr()
{
	return m_pDIB;
}

void ImageFile::GetSize( int& width , int& height )
{
	width  =  m_iWidth;
	height =  m_iHeight;
}