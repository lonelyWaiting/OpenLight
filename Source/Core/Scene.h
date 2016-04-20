#pragma once

#include "PCH.h"
#include "Primitive/Primitive.h"
#include "Math/Ray.h"
#include "Light/Light.h"
#include "Spectrum/Spectrum.h"
#include "VObject.h"

class Scene : public VObject , public VIntersectable
{
public:
	Scene();

	void AddObject( Primitive& prim );
	
	void AddLight( Light* light );

	const Primitive& GetPrimitive( int index ) const;

	const std::vector<Light*>& GetLights() const;

	bool Intersect( const Ray& ray , IntersectRecord* record ) const;

public:
	virtual void Deserialization( XMLElement* RootElement );

public:
	int GetObjectCount() const;

protected:
	std::vector<Primitive> Objects;

	std::vector<Light*> lights;
};