#include "Utilities/PCH.h"
#include "Math/Bound3.h"
#include "Math/Ray.h"
#include "Primitive/IntersectRecord.h"
#include "ModelParser.h"
#include "Light/Light.h"
#include "tinyxml2.h"
#include "TriangleMesh.h"
#include "Utilities/srString.h"

TriangleMesh::TriangleMesh()
{
	points        = nullptr;
	normals       = nullptr;
	triangles     = nullptr;
	TriangleCount = 0;
	VertexNum     = 0;
	bSubShape = true;
}

TriangleMesh::TriangleMesh( const Transform* ObjectToWorld , Point3f* _points , Normal* _normals , Vector2f* _uvs , Triangle* _triangles , int _VertexNum , int _TriangleCount )
	: Shape( ObjectToWorld )
	, points( _points )
	, normals( _normals )
	, triangles( _triangles )
	, uvs( _uvs )
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
	// ´ÓWorld Space±ä»»µ½Object Space
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

void TriangleMesh::Deserialization( tinyxml2::XMLElement* ShapeRootElement )
{
	const char* name = ShapeRootElement->Attribute( "filename" );
	filename = new char[strlen( name )];
	strcpy( filename , name );

	ParseVector3( ShapeRootElement->FirstChildElement( "transform" )->Attribute( "position" ) , &Pos[0] );

	ModelParse( filename , points , normals , uvs , triangles , VertexNum , TriangleCount );

	*ObjectToWorld = Translate( Vector3f( Pos ) );
	*WorldToObject = Inverse( *ObjectToWorld );

	for( int i = 0; i < TriangleCount; i++ )
	{
		triangles[i].SetTriangleMesh( this );
	}

	for( int i = 0; i < VertexNum; i++ )
	{
		BBoxLocal.ExpendToInclude( points[i] );
	}

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}


void TriangleMesh::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	{
		pRootElement->SetAttribute( "type" , GetName() );
	}
	
	{
		tinyxml2::XMLElement* pFilenameElement = xmlDoc.NewElement( "filename" );

		pFilenameElement->SetText( filename );

		pRootElement->InsertEndChild( pFilenameElement );
	}

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , Pos.x , Pos.y , Pos.z );

		tinyxml2::XMLElement* pTransformElement = xmlDoc.NewElement( "transform" );

		pTransformElement->SetAttribute( "position" , pText );

		pRootElement->InsertEndChild( pTransformElement );

		SAFE_DELETE( pText );
	}
}

int TriangleMesh::GetChildCount() const
{
	return TriangleCount;
}

Shape* TriangleMesh::GetChild( int index ) const
{
	return &triangles[index];
}