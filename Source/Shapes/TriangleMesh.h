#pragma once

#include "Shape.h"
#include "Math/Normal.h"
#include "Math/Point2.h"
#include "Triangle.h"

class TriangleMesh : public Shape
{
public:
	TriangleMesh();

	TriangleMesh( const Transform* ObjectToWorld , const Transform* WorldToObject , Point3f* _points , Normal* _normals , int _TriangleCount );

	~TriangleMesh();

	Bound3f ObjectBound() const;

	bool Intersect( const Ray& ray , IntersectRecord* record ) const;

	bool IntersectP( const Ray& ray ) const;

public:
	void ParseShape( XMLElement* ShapeRootElement );

private:
	Point3f * points;
	Normal  * normals;
	Triangle* triangles;
	int		  VertexNum;
	int		  TriangleCount;
};