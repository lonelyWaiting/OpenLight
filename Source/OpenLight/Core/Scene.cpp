#include "Utilities/PCH.h"
#include "tinyxml2.h"
#include "Scene.h"
#include "Texture/ConstantEnvironment.h"

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
	if( light == nullptr )
	{
		return;
	}

	lights.push_back( light );
}

bool Scene::Intersect( const Rayf& ray , IntersectRecord* record ) const
{
	bool bHit = false;

	Rayf r( ray.Origin , ray.Direction , ray.MinT , ray.MaxT , ray.time , ray.depth );

	for( unsigned int i = 0; i < Objects.size(); i++ )
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

void Scene::Deserialization( tinyxml2::XMLElement* RootElement )
{
	tinyxml2::XMLElement* pEnvironmentElement = RootElement->FirstChildElement( "Environment" );
	if( pEnvironmentElement )
	{
		pEnvironment = Environment::Create( pEnvironmentElement->Attribute( "type" ) );
		pEnvironment->Deserialization( pEnvironmentElement );
	}
	else
	{
		pEnvironment = new ConstantEnvironment;
	}

	tinyxml2::XMLElement* PrimitiveElement = RootElement->FirstChildElement( "primitive" );
	while( PrimitiveElement )
	{
		Primitive* primitive = new Primitive;
		Assert( primitive != nullptr );
		primitive->Deserialization( PrimitiveElement );
		AddObject( *primitive );
		AddLight( primitive->GetAreaLight() );
		PrimitiveElement = PrimitiveElement->NextSiblingElement( "primitive" );
	}
}

void Scene::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	{
		tinyxml2::XMLElement* pEnvironmentElement = xmlDoc.NewElement( "Environment" );
		
		pEnvironment->Serialization( xmlDoc , pEnvironmentElement );

		pRootElement->InsertEndChild( pEnvironmentElement );
	}
	
	
	for( auto& object : Objects )
	{
		tinyxml2::XMLElement* pElement = xmlDoc.NewElement( "primitive" );

		pRootElement->InsertEndChild( pElement );

		object.Serialization( xmlDoc , pElement );
	}

	for( auto& light : lights )
	{
		if( !strcmp( light->GetName() , "AreaLight" ) )
		{
			// AreaLight只会挂接在Primitive下，由Primitive序列化
			continue;
		}

		tinyxml2::XMLElement* pElement = xmlDoc.NewElement( "light" );

		pRootElement->InsertEndChild( pElement );

		light->Serialization( xmlDoc , pElement );
	}
}

int Scene::GetObjectCount() const
{
	return Objects.size();
}

const Primitive& Scene::GetPrimitive( int index ) const
{
	return Objects[index];
}

Light* Scene::GetLight( int index ) const
{
	return lights[index];
}

Environment* Scene::GetEnvironmentPtr() const
{
	return pEnvironment;
}

void Scene::AddEnvironment( Environment* _pEnvironment )
{
	pEnvironment = _pEnvironment;
}