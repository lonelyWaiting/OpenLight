#include "Utilities/PCH.h"
#include "tinyxml2.h"
#include "Scene.h"
#include "Texture/ConstantEnvironment.h"
#include "Renderer/Renderer.h"
#include "Camera/Camera.h"
#include "Sampler/Sampler.h"
#include "Integrator/SurfaceIntegrator.h"

Scene::Scene()
{
	mCameraPtr = nullptr;

	mRendererPtr = nullptr;

	mSurfaceIntegratorPtr = nullptr;

	mSamplerPtr = nullptr;

	mEnvironmentPtr = nullptr;

	InitRTTI();
}

void Scene::AddEntity( Primitive* prim )
{
	mEntityList.push_back( prim );

	BBoxLocal.ExpendToInclude( prim->GetObjectBoundingBox() );
	BBoxWorld.ExpendToInclude( prim->GetWorldBoundingBox() );
}

void Scene::AddLight( Light* light )
{
	if( light == nullptr )
	{
		return;
	}

	mLightList.push_back( light );
}

void Scene::AddEnvironment( Environment* pEnvironment )
{
	mEnvironmentPtr = pEnvironment;
}

SurfaceIntegrator* Scene::GetSurfaceIntegrator() const
{
	return mSurfaceIntegratorPtr;
}

Camera* Scene::GetCamera() const
{
	return mCameraPtr;
}

Renderer* Scene::GetRenderer() const
{
	return mRendererPtr;
}

Environment* Scene::GetEnvironmentPtr() const
{
	return mEnvironmentPtr;
}

Light* Scene::GetLight( unsigned int index ) const
{
	return mLightList[index];
}

Primitive* Scene::GetEntity( unsigned int index ) const
{
	return mEntityList[index];
}

bool Scene::Intersect( const Rayf& ray , IntersectRecord* record ) const
{
	bool bHit = false;

	Rayf r( ray.Origin , ray.Direction , ray.MinT , ray.MaxT , ray.time , ray.depth );

	for( unsigned int i = 0; i < mEntityList.size(); i++ )
	{
		if( mEntityList[i]->Intersect( r , record ) )
		{
			bHit = true;
		}
	}

	return bHit;
}

void Scene::RemoveScene()
{
	// ------------------------Environment----------------------------------
	SAFE_DELETE( mEnvironmentPtr );

	// -------------------------Primitive-----------------------------------
	{
		auto it = mEntityList.begin();

		while( it != mEntityList.end() )
		{
			SAFE_DELETE( *it );
			it = mEntityList.erase( it );
		}
	}

	// -------------------------Light---------------------------------------
	{
		auto it = mLightList.begin();

		while( it != mLightList.end() )
		{
			SAFE_DELETE( *it );
			it = mLightList.erase( it );
		}
	}

	// ------------------------Camera--------------------------------------
	{
		SAFE_DELETE( mCameraPtr );
	}

	// -------------------------Integrator-------------------------------
	{
		SAFE_DELETE( mSurfaceIntegratorPtr );
	}

	// --------------------------Renderer-------------------------------
	{
		SAFE_DELETE( mRendererPtr );
	}

	// --------------------------Sampler-------------------------------
	{
		SAFE_DELETE( mSamplerPtr );
	}
}

int Scene::GetEntityCount() const
{
	return mEntityList.size();
}

int Scene::GetLightCount() const
{
	return mLightList.size();
}

bool Scene::Render()
{
	return mRendererPtr ? mRendererPtr->Render(this) : false;
}

#include "Camera/Camera.h"
#include "Film/Film.h"

const void* Scene::GetFilmData(Vector2f& size)
{
	if (!mCameraPtr)	return nullptr;

	Film* film = mCameraPtr->GetFilm();
	if (!film)	return nullptr;

	Image* image = film->GetImage();
	if (!image)	return nullptr;

	size = film->GetResolution();

	return image->GetData();
}