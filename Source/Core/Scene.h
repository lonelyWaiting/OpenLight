#pragma once

#include "PCH.h"
#include "Primitive/Primitive.h"
#include "Math/Ray.h"
#include "Light/Light.h"
#include "Spectrum/Spectrum.h"

class Scene
{
public:
	Scene();

	void AddObject( Primitive& prim );
	
	void AddLight( Light* light );

	/*Spectrum Trace( const Ray& ray ) const;*/

	// 相交计算，将相交信息保存在IntersectRecord中
	bool Intersect( const Ray& ray , IntersectRecord* record ) const;

	// 相交测试，只判断是否相交，并不保存相交信息
	bool IntersectP( const Ray& ray ) const;

	const std::vector<Light*>& GetLights() const;

protected:
	std::vector<Primitive> Objects;

	std::vector<Light*> lights;
};