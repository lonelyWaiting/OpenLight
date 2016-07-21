#pragma once

#include "Shape.h"
#include "Triangle.h"
#include "Math/MathHelper.h"
#include "Math/Point3.h"

namespace tinyxml2
{
	class XMLElement;
}

class TriangleMesh : public Shape
{
	DECLARE_DYNAMIC_CREATE_DERIVED( TriangleMesh , Shape )

public:
	TriangleMesh();

	TriangleMesh( const Transform* ObjectToWorld ,  Point3f* _points , Vector3f* _normals , Vector2f* _uvs , Triangle* _triangles , int _VertexNum , int _TriangleCount );

	~TriangleMesh();

	bool Intersect( Rayf& ray , IntersectRecord* record ) const;

public:
	void Deserialization( tinyxml2::XMLElement* ShapeRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	int GetChildCount() const;

	Shape* GetChild( int index ) const;

private:
	Point3f *	mPointList;
	Vector3f  *	mNormalList;
	Vector2f*	mTexcoordList;
	Triangle*	mTriangleList;
	int			mVertexCount;
	int			mTriangleCount;

	Point3f		mWorldPos;
	char*		mFilename;

	friend class Triangle;
};