#pragma once

#include "Shape.h"

class Ray;
class IntersectRecord;

class AggregateShape : public Shape
{
public:
	void AddShape( Shape* shape );

	bool Intersect( Ray& ray , IntersectRecord* record ) const;

public:
	void Deserialization( XMLElement* RootElement );

protected:
	mutable std::vector<Shape*> shapes;
};