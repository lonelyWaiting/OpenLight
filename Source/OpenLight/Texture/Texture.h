#ifndef TEXTURE_H
#define TEXTURE_H

#include "Spectrum/Spectrum.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

class Texture
{
public:
	virtual Spectrum GetColor( const Vector2f& uv , const Vector3f& p ) const = 0;
};
#endif