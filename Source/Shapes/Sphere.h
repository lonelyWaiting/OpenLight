#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Bound3.h"
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();

	Sphere( Point3f Center , double radius );

	~Sphere();

	Bound3f ObjectBound() const;

	bool Intersect( Ray& ray , IntersectRecord* record ) const;

	bool IntersectP( const Ray& ray ) const;

public:
	void ParseShape( XMLElement* ShapeRootElement );

private:
	double m_Radius;
	Point3f m_Center;
};

#endif