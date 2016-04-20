#include "PCH.h"
#include "Scene.h"

Scene::Scene()
{

}

void Scene::AddObject( Primitive& prim )
{
	Objects.push_back( prim );

	BBoxLocal.ExpendToInclude( prim.GetObjectBoundingBox() );
	BBoxWorld.ExpendToInclude( prim.GetWorldBoundingBox() );
}

void Scene::AddLight( Light* light )
{
	lights.push_back( light );
}

bool Scene::Intersect( const Ray& ray , IntersectRecord* record ) const
{
	bool bHit = false;

	Ray r( ray.Origin , ray.Direction , ray.MinT , ray.MaxT , ray.time , ray.depth );

	for( int i = 0; i < Objects.size(); i++ )
	{
		if( Objects[i].Intersect( r , record ) )
		{
			bHit = true;
		}
	}

	return bHit;
}

const std::vector<Light*>& Scene::GetLights() const
{
	return lights;
}

void Scene::Deserialization( XMLElement* RootElement )
{

}

int Scene::GetObjectCount() const
{
	return Objects.size();
}

const Primitive& Scene::GetPrimitive( int index ) const
{
	return Objects[i];
}