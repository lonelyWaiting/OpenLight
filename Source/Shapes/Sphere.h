#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Bound3.h"
#include "Shape.h"

namespace tinyxml2
{
	class XMLElement;
}

class Sphere : public Shape
{
	DECLARE_DYNAMIC_CREATE_DERIVED(Sphere , Shape)

public:
	Sphere();

	Sphere( Point3f Center , double radius );

	~Sphere();

	bool Intersect( Ray& ray , IntersectRecord* record ) const;

	double Area() const;
	
	double PDF( const Point3f& p , const Vector3f& wi ) const;

	Point3f Sample( const Point3f& p , LightSample& lightSample , Normal& normalSample );

public:
	void Deserialization( tinyxml2::XMLElement* ShapeRootElement );

private:
	double m_Radius;
	Point3f m_Center;
};

#endif