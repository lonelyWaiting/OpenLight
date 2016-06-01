#pragma once

#include "Texture/Texture.h"

class NoiseTexture
{
public:
	virtual Spectrum GetColor( const Vector2f& uv , const Vector3f& p ) const;
};