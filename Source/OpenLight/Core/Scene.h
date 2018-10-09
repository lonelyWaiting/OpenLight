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

class Renderer;
class SurfaceIntegrator;
class Sampler;
class Camera;
class Environment;

class Scene : public VSerializableObject , public VIntersectable
{
public:
	Scene();

	void AddEntity( Primitive* prim );
	
	void AddLight( Light* light );

	void AddEnvironment( Environment* );

public:
	void InitRTTI();

	bool Render();

public:
	SurfaceIntegrator*	GetSurfaceIntegrator() const;
	
	Camera*				GetCamera() const;
	
	Renderer*			GetRenderer() const;
	
	Environment*		GetEnvironmentPtr() const;
	
	Light* GetLight( unsigned int index ) const;

	Primitive* GetEntity( unsigned int index ) const;

	const void* GetFilmData(Vector2f& size);

public:
	bool Intersect( const Rayf& ray , IntersectRecord* record ) const;

public:
	void RemoveScene();

public:
	virtual void Serialization( const char* filename = nullptr );

	virtual void Deserialization( const char* filename = nullptr );

public:
	int GetEntityCount() const;

	int GetLightCount() const;

protected:
	std::vector<Primitive*>	mEntityList;
	std::vector<Light*>		mLightList;
	SurfaceIntegrator*		mSurfaceIntegratorPtr;
	Environment*			mEnvironmentPtr;
	Renderer*				mRendererPtr;
	Sampler*				mSamplerPtr;
	Camera*					mCameraPtr;
	std::string				mFilename;
};