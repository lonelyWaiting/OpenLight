#ifndef VShaderHelper_H
#define VShaderHelper_H

#include <string>
#include <vector>
#include <fstream>

typedef unsigned char byte;

void LoadShader( std::string& filename , std::vector<byte> &blob )
{
	std::ifstream ShaderFile( filename , std::ios::in | std::ios::binary | std::ios::ate );

	if( ShaderFile.is_open() )
	{
		int Length = ( int )ShaderFile.tellg();

		blob.resize( Length );

		ShaderFile.seekg( 0 , std::ios::beg );
		ShaderFile.read( reinterpret_cast< char* >( &blob[0] ) , blob.size() );
		ShaderFile.close();
	}
}
#endif