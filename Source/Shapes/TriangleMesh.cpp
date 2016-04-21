#include "PCH.h"
#include "Math/Bound3.h"
#include "Math/Ray.h"
#include "Primitive/IntersectRecord.h"
#include "ModelParser.h"
#include "TriangleMesh.h"

TriangleMesh::TriangleMesh()
{
	points        = nullptr;
	normals       = nullptr;
	triangles     = nullptr;
	TriangleCount = 0;
	VertexNum     = 0;
	bCombination  = true;
}

TriangleMesh::TriangleMesh( const Transform* ObjectToWorld , Point3f* _points , Normal* _normals , Triangle* _triangles , int _VertexNum , int _TriangleCount )
	: Shape( ObjectToWorld )
	, points( _points )
	, normals( _normals )
	, triangles( _triangles )
	, VertexNum( _VertexNum )
	, TriangleCount( _TriangleCount )
{
	for( int i = 0; i < VertexNum; i++ )
	{
		BBoxLocal.ExpendToInclude( points[i] );
	}

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}

TriangleMesh::~TriangleMesh()
{

}

bool TriangleMesh::Intersect( Ray& ray , IntersectRecord* record ) const
{
	// 从World Space变换到Object Space
	Ray r = ( *WorldToObject )( ray );

	int bHitIndex = -1;

	for( int i = 0; i < TriangleCount; i++ )
	{
		if( triangles[i].Intersect( r , record ) )
		{
			bHitIndex = i;
		}
	}

	if( bHitIndex != -1 )
	{
		ray.MaxT = r.MaxT;
		record->ObjectToWorld = *ObjectToWorld;
		record->WorldToObject = *WorldToObject;
		record->normal = normals[3 * bHitIndex];
		return true;
	}

	return false;
}

void TriangleMesh::Deserialization( XMLElement* ShapeRootElement )
{
	const char* filename = ShapeRootElement->FirstChildElement( "filename" )->GetText();

	XMLElement* ShapeTransformElement = ShapeRootElement->FirstChildElement( "transform" );

	double x , y , z;

	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "x" )->QueryDoubleText( &x );
	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "y" )->QueryDoubleText( &y );
	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "z" )->QueryDoubleText( &z );

	*ObjectToWorld = Translate( Vector3f( x , y , z ) );
	*WorldToObject = Inverse( *ObjectToWorld );

	double r , g , b;
	XMLElement* ShapeEmmisiveElement = ShapeRootElement->FirstChildElement( "emmisive" );
	ShapeEmmisiveElement->FirstChildElement( "r" )->QueryDoubleText( &r );
	ShapeEmmisiveElement->FirstChildElement( "g" )->QueryDoubleText( &g );
	ShapeEmmisiveElement->FirstChildElement( "b" )->QueryDoubleText( &b );

	emmisive = Spectrum::FromRGB( r , g , b );

	// 解析OBJ模型
	ModelParse( filename , points , normals , triangles , VertexNum , TriangleCount );

	for( int i = 0; i < TriangleCount; i++ )
	{
		triangles[i].SetPrimitive( pPrimitive );
		triangles[i].SetTriangleMesh( this );
	}

	for( int i = 0; i < VertexNum; i++ )
	{
		BBoxLocal.ExpendToInclude( points[i] );
	}

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}

int TriangleMesh::GetSubShapeCount() const
{
	return TriangleCount;
}

Shape* TriangleMesh::GetSubShape( int index ) const
{
	return &triangles[index];
}