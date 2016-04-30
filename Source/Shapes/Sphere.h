#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Bound3.h"
#include "Shape.h"

class Sphere : public Shape
{
	DECLARE_DYNAMIC_CREATE_DERIVED(Sphere , Shape)

public:
	Sphere();

	Sphere( Point3f Center , double radius );

	~Sphere();

	bool Intersect( Ray& ray , IntersectRecord* record ) const;

public:
	void Deserialization( XMLElement* ShapeRootElement );

private:
	double m_Radius;
	Point3f m_Center;
};

#endif