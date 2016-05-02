#pragma once

#include "Shape.h"
#include "Math/Normal.h"
#include "Math/Point2.h"
#include "Triangle.h"

class TriangleMesh : public Shape
{
	DECLARE_DYNAMIC_CREATE_DERIVED( TriangleMesh , Shape )

public:
	TriangleMesh();

	TriangleMesh( const Transform* ObjectToWorld ,  Point3f* _points , Normal* _normals , Triangle* _triangles , int _VertexNum , int _TriangleCount );

	~TriangleMesh();

	bool Intersect( Ray& ray , IntersectRecord* record ) const;

public:
	void Deserialization( XMLElement* ShapeRootElement );

public:
	int GetChildCount() const;

	Shape* GetChild( int index ) const;

private:
	Point3f * points;
	Normal  * normals;
	Triangle* triangles;
	int		  VertexNum;
	int		  TriangleCount;

	friend class Triangle;
};