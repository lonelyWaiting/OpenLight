#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Bound3.h"
#include "Shape.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

class Sphere : public Shape
{
	DECLARE_DYNAMIC_CREATE_DERIVED(Sphere , Shape)

public:
	Sphere();

	Sphere( Point3f Center , float radius );

	~Sphere();

	bool Intersect( Rayf& ray , IntersectRecord* record ) const;

	float Area() const;
	
	float PDF( const Point3f& p , const Vector3f& wi ) const;

	Point3f Sample( const Point3f& p , LightSample& lightSample , Vector3f& normalSample );

public:
	void Deserialization( tinyxml2::XMLElement* ShapeRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );
	
public:
	// -------------------------------Information-----------------------------------------
	float GetRadius();

	void SetRadius( float radius );

private:
	float m_Radius;
};

#endif