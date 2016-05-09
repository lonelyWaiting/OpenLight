#include "IEffect.h"

IEffect::IEffect()
{

}

IEffect::~IEffect()
{

}

std::vector<byte> IEffect::LoadShader( std::string filename )
{
    std::vector<byte> FileData;

    std::ifstream ShaderFile( filename , std::ios::in | std::ios::binary | std::ios::ate );

    if ( ShaderFile.is_open() )
    {
        int Length = (int)ShaderFile.tellg();

        FileData.resize( Length );

        ShaderFile.seekg( 0 , std::ios::beg );
        ShaderFile.read( reinterpret_cast<char*>( &FileData[0] ) , FileData.size() );
        ShaderFile.close();
    }

    return FileData;
}