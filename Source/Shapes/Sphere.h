#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Bound3.h"
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();

	Sphere( const Transform* ObjectToWorld , const Transform* WorldToObject , double radius );

	~Sphere();

	Bound3f ObjectBound() const;

	bool Intersect( const Ray& ray , IntersectRecord* record ) const;

	bool IntersectP( const Ray& ray ) const;

public:
	void ParseShape( XMLElement* ShapeRootElement );

private:
	double m_Radius;
};

#endif