#include "Utilities/PCH.h"
#include "srString.h"
#include <codecvt>
#include <locale>

srString::srString()
{

}

std::string srString::ToAscii( const std::wstring& input )
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t> , wchar_t>().to_bytes( input );
}

std::wstring srString::ToUnicode( const std::string& input )
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t> , wchar_t>().from_bytes( input );
}

void ParseVector( const std::string s , float* v )
{
	std::vector<std::string> elements;
	std::stringstream ss( s );
	std::string item;

	while( std::getline( ss , item , ',' ) )
	{
		elements.push_back( item );
	}

	//assert( elements.size() >= 1 && elements.size() <= 3 );

	for( int i = 0; i < elements.size(); i++ )
	{
		v[i] = atof( elements[i].c_str() );
	}
}