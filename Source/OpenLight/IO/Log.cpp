#include "FileSystem.h"
#include <Utilities/PCH.h>
#include <Utilities/srString.h>
#include "Log.h"
#include <stdarg.h>

Log::Log()
{
	Open();
}

Log::~Log()
{
	Close();
}

Log& Log::Get()
{
	static Log log;
	return log;
}

void Log::Open()
{
	FileSystem fs;

	std::wstring FilePath = fs.GetLogFolder() + std::wstring( L"Log.html" );

	LogFile.imbue( std::locale( "" , std::locale::all ^ std::locale::numeric ) );

	LogFile.open( FilePath.c_str() );

	Write( std::wstring( L"<html><head><title></title></head><body bgcolor=\"#C0C0C0\">" ) );
}

void Log::Close()
{
	Write( std::wstring( L"</body></html>" ) );
	LogFile.flush();
}

void Log::Write( std::wstring& TextString )
{
	LogFile << TextString.c_str();
}

void Log::Error( const wchar_t* format , ... )
{
	wchar_t errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#FF0000\">" + std::wstring( L"Error: " ) + std::wstring( errorBuf ) + L"\0<br /></p>" );
}

void Log::Error( std::wstring format , ... )
{
	wchar_t errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format.c_str() , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#FF0000\">" + std::wstring( L"Error: " ) + std::wstring( errorBuf ) + L"\0<br /></p>" );
}

void Log::Error( const char* format , ... )
{
	char errorBuf[2048];
	va_list args;
	va_start( args , format );
	processError( format , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#FF0000\">" + std::wstring( L"Error: " ) + srString::ToUnicode( std::string( errorBuf ) ) + L"\0<br /></p>" );
}

void Log::Warning( const wchar_t* format , ... )
{
	wchar_t errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#FFD700\">" + std::wstring( L"Warning: " ) + std::wstring( errorBuf ) + L"\0<br /></p>" );
}

void Log::Warning( std::wstring format , ... )
{
	wchar_t errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format.c_str() , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#FFD700\">" + std::wstring( L"Warning: " ) + std::wstring( errorBuf ) + L"\0<br /></p>" );
}

void Log::Warning( const char* format , ... )
{
	char errorBuf[2048];
	va_list args;
	va_start( args , format );
	processError( format , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#FFD700\">" + std::wstring( L"Warning: " ) + srString::ToUnicode( std::string( errorBuf ) ) + L"\0<br /></p>" );
}

void Log::Info( const wchar_t* format , ... )
{
	wchar_t errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#1E90FF\">" + std::wstring( L"Info: " ) + std::wstring( errorBuf ) + L"\0<br /></p>" );
}

void Log::Info( std::wstring format , ... )
{
	wchar_t errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format.c_str() , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#1E90FF\">" + std::wstring( L"Info: " ) + std::wstring( errorBuf ) + L"\0<br /></p>" );
}

void Log::Info( const char* format , ... )
{
	char errorBuf[2048];
	va_list args;
	va_start( args , format );
	processError( format , args , errorBuf );
	va_end( args );

	Write( L"<p style=\"color:#1E90FF\">" + std::wstring( L"Info: " ) + srString::ToUnicode( std::string( errorBuf ) ) + L"\0<br /></p>" );
}

template<int size>
void Log::process( const wchar_t* format , va_list args , wchar_t( &errorBuf )[size] )
{
	_vsnwprintf_s( errorBuf , size , _TRUNCATE , format , args );
}

template<int size>
void Log::processError( const char* format , va_list args , char( &errorBuf )[size] )
{
	_vsnprintf_s( errorBuf , size , _TRUNCATE , format , args );
}