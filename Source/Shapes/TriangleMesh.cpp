#include "PCH.h"
#include "Math/Bound3.h"
#include "Math/Ray.h"
#include "Primitive/IntersectRecord.h"
#include "ModelParser.h"
#include "TriangleMesh.h"

TriangleMesh::TriangleMesh()
{

}

TriangleMesh::TriangleMesh( const Transform* ObjectToWorld , const Transform* WorldToObject , Point3f* _points , Normal* _normals , int _TriangleCount )
{

}

TriangleMesh::~TriangleMesh()
{

}

Bound3f TriangleMesh::ObjectBound() const
{
	Bound3f bbox;

	for( int i = 0; i < VertexNum; i++ )
	{
		bbox.ExpendToInclude( points[i] );
	}

	return bbox;
}

bool TriangleMesh::Intersect( const Ray& ray , IntersectRecord* record ) const
{
	// 从World Space变换到Object Space
	Ray r = ( *WorldToObject )( ray );

	bool bHit = false;

	for( int i = 0; i < TriangleCount; i++ )
	{
		if( triangles[i].Intersect( r , points , record ) )
		{
			r.MaxT = record->HitT;
			bHit = true;
		}
	}

	if( bHit )
	{
		record->ObjectToWorld = *ObjectToWorld;
		record->WorldToObject = *WorldToObject;

		return true;
	}

	return false;
}

bool TriangleMesh::IntersectP( const Ray& ray ) const
{
	Ray r = ( *WorldToObject )( ray );

	bool bHit = false;

	for( int i = 0; i < TriangleCount; i++ )
	{
		if( triangles[i].IntersectP( r , points ) )
		{
			return true;
		}
	}

	return false;
}

void TriangleMesh::ParseShape( XMLElement* ShapeRootElement )
{
	const char* filename = ShapeRootElement->FirstChildElement( "filename" )->GetText();

	XMLElement* ShapeTransformElement = ShapeRootElement->FirstChildElement( "transform" );

	float x , y , z;

	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "x" )->QueryFloatText( &x );
	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "y" )->QueryFloatText( &y );
	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "z" )->QueryFloatText( &z );

	*ObjectToWorld = Translate( Vector3f( x , y , z ) );
	*WorldToObject = Inverse( *ObjectToWorld );

	// 解析OBJ模型
	ModelParser( filename , points , normals , triangles , VertexNum , TriangleCount );
}