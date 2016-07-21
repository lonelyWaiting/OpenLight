#pragma once

#include "Shape.h"

struct IntersectRecord;
namespace tinyxml2
{
	class XMLElement;
}

class AggregateShape : public Shape
{
public:
	void AddShape( Shape* shape );

	bool Intersect( Rayf& ray , IntersectRecord* record ) const;

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

protected:
	mutable std::vector<Shape*> shapes;
};