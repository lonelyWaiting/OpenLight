#ifndef EFFECT_H
#define EFFECT_H

#include "d3dUtil.h"

class IEffect
{
public:
    IEffect();
    virtual ~IEffect();

protected:
    std::vector<byte> LoadShader( std::string filename );
};

#endif