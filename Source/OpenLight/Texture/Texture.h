#ifndef TEXTURE_H
#define TEXTURE_H

#include "Spectrum/Spectrum.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Core/VSerializableObject.h"
#include "Core/VCustomRTTI.h"

class Texture : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( Texture )

public:
	// 通用接口，根据位置和纹理坐标返回相应的纹理值
	virtual Spectrum Evalute( const Vector2f& uv , const Point3f& p ) const = 0;
	virtual ~Texture();
};
#endif