#pragma once

#include "Utilities/PCH.h"

class Log
{
public:
	static Log& Get();

	void Open();

	void Close();
	
	void Error( const wchar_t* format , ... );

	void Warning( const wchar_t* foramt , ... );

	void Info( const wchar_t* format , ... );

	void Error( std::wstring format , ... );

	void Warning( std::wstring format , ... );

	void Info( std::wstring foramt , ... );

	void Error( const char* format , ... );

	void Warning( const char* format , ... );

	void Info( const char* format , ... );

private:
	template<int size>
	void process( const wchar_t* format , va_list args , wchar_t( &errorBuf )[size] );

	template<int size>
	void processError( const char* format , va_list args , char( &errorBuf )[size] );

	void Write( std::wstring& TextString );
	
protected:
	Log();

	~Log();

	std::wofstream LogFile;
};