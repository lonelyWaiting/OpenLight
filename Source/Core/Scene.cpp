#include "PCH.h"
#include "Scene.h"

Scene::Scene()
{

}

void Scene::AddObject( Primitive& prim )
{
	Objects.push_back( prim );
}

Spectrum Scene::Trace( const Ray& ray ) const
{
	int size = Objects.size();

	IntersectRecord record;

	float MaxT = ray.MaxT;

	bool bHit = false;

	for( int i = 0; i < size; i++ )
	{
		// 对每个物体进行相交测试
		if( Objects[i].Intersect( ray , &record ) )
		{
			if( record.HitT < MaxT )
			{
				ray.MaxT = record.HitT;
				bHit = true;
			}
		}
	}

	return record.HitPointColor;
}