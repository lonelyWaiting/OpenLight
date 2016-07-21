#pragma once

#include "Utilities/PCH.h"
#include "Primitive/Primitive.h"
#include "Math/Ray.h"
#include "Light/Light.h"
#include "Spectrum/Spectrum.h"
#include "VSerializableObject.h"
#include "Texture/Environment.h"

namespace tinyxml2
{
	class XMLDocument;
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

	bool Intersect( const Rayf& ray , IntersectRecord* record ) const;

	Environment* GetEnvironmentPtr() const;

	void AddEnvironment( Environment* );

public:
	virtual void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

	virtual void Deserialization( tinyxml2::XMLElement* RootElement );

public:
	int GetObjectCount() const;

protected:
	std::vector<Primitive> Objects;

	std::vector<Light*> lights;

	Environment* pEnvironment;
};