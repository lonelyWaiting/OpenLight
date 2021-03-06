#pragma once

#include "Utilities/PCH.h"
#include <array>

class srString
{
public:
	static std::string ToAscii( const std::wstring& input );
	static std::wstring ToUnicode( const std::string& input );

private:
	srString();
};

template<typename T>
inline std::wstring ToString( const T& value )
{
	std::wostringstream stream;
	
	if( !( stream << value ) )
	{
		return L"";
	}

	return stream.str();
}

void ParseVector( const std::string s , float* v );