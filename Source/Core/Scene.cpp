#include "PCH.h"
#include "Scene.h"

Scene::Scene()
{

}

void Scene::AddObject( Primitive& prim )
{
	Objects.push_back( prim );
}

void Scene::AddLight( Light* light )
{
	lights.push_back( light );
}

Spectrum Scene::Trace( const Ray& ray ) const
{
	int size = Objects.size();

	IntersectRecord record;

	float MaxT = ray.MaxT;

	bool bHit = false;

	Ray r( ray.Origin , ray.Direction , ray.MinT , ray.MaxT , ray.time , ray.depth );

	for( int i = 0; i < size; i++ )
	{
		// 对每个物体进行相交测试
		if( Objects[i].Intersect( r , &record ) )
		{
			if( record.HitT < r.MaxT )
			{
				r.MaxT = record.HitT;
				bHit = true;
			}
		}
	}

	return record.HitPointColor;
}

bool Scene::Intersect( const Ray& ray , IntersectRecord* record ) const
{
	bool bHit = false;

	Ray r( ray.Origin , ray.Direction , ray.MinT , ray.MaxT , ray.time , ray.depth );

	for( int i = 0; i < Objects.size(); i++ )
	{
		if( Objects[i].Intersect( r , record ) )
		{
			r.MaxT = record->HitT;

			bHit = true;
		}
	}

	return bHit;
}

bool Scene::IntersectP( const Ray& ray ) const
{
	bool bHit = false;

	Ray r( ray.Origin , ray.Direction , ray.MinT , ray.MaxT , ray.time , ray.depth );

	for( int i = 0; i < Objects.size(); i++ )
	{
		if( Objects[i].IntersectP( r ) )
		{
			bHit = true;
			break;
		}
	}

	return bHit;
}

const std::vector<Light*>& Scene::GetLights() const
{
	return lights;
}