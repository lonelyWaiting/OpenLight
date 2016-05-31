#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
	virtual Spectrum GetColor( const Vector2f& uv , const Vector3f& p ) const = 0;
};
#endif