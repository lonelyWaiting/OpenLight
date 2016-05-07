#pragma once

#include "Utilities/PCH.h"
#include "Primitive/Primitive.h"
#include "Math/Ray.h"
#include "Light/Light.h"
#include "Spectrum/Spectrum.h"
#include "VSerializableObject.h"

namespace tinyxml2
{
	class XMLElement;
}

class Scene : public VSerializableObject , public VIntersectable
{
public:
	Scene();

	void AddObject( Primitive& prim );
	
	void AddLight( Light* light );

	const Primitive& GetPrimitive( int index ) const;

	const std::vector<Light*>& GetLights() const;

	Light* GetLight( int index ) const;

	bool Intersect( const Ray& ray , IntersectRecord* record ) const;

public:
	virtual void Deserialization( tinyxml2::XMLElement* RootElement );

public:
	int GetObjectCount() const;

protected:
	std::vector<Primitive> Objects;

	std::vector<Light*> lights;
};