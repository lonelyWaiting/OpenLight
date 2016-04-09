#include "PCH.h"
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