#include "Utilities/PCH.h"
#include "Math/Ray.h"
#include "Primitive/IntersectRecord.h"
#include "AggregateShape.h"

void AggregateShape::AddShape( Shape* shape )
{
	shapes.push_back( shape );
}

bool AggregateShape::Intersect( Rayf& ray , IntersectRecord* record ) const
{
	int count = shapes.size();

	bool bHit = false;

	for( std::vector<Shape*>::iterator iter = shapes.begin(); iter != shapes.end(); iter++ )
	{
		if( ( *iter )->Intersect( ray , record ) )
		{
			bHit = true;
		}
	}

	return bHit;
}

void AggregateShape::Deserialization( tinyxml2::XMLElement* RootElement )
{
	// nothing to do
}