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
	mPointList        = nullptr;
	mNormalList       = nullptr;
	mTriangleList     = nullptr;
	mTriangleCount = 0;
	mVertexCount     = 0;
	mHasSubShape = true;
}

TriangleMesh::TriangleMesh( const Transform* ObjectToWorld , Point3f* _points , Vector3f* _normals , Vector2f* _uvs , Triangle* _triangles , int _VertexNum , int _TriangleCount )
	: Shape( ObjectToWorld )
	, mPointList( _points )
	, mNormalList( _normals )
	, mTriangleList( _triangles )
	, mTexcoordList( _uvs )
	, mVertexCount( _VertexNum )
	, mTriangleCount( _TriangleCount )
{
	for( int i = 0; i < mVertexCount; i++ )
	{
		BBoxLocal.ExpendToInclude( mPointList[i] );
	}

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}

TriangleMesh::~TriangleMesh()
{

}

bool TriangleMesh::Intersect( Rayf& ray , IntersectRecord* record ) const
{
	// ´ÓWorld Space±ä»»µ½Object Space
	Rayf r = ( *mWorldToObject )( ray );

	int bHitIndex = -1;

	for( int i = 0; i < mTriangleCount; i++ )
	{
		if( mTriangleList[i].Intersect( r , record ) )
		{
			bHitIndex = i;
		}
	}

	if( bHitIndex != -1 )
	{
		ray.MaxT = r.MaxT;
		record->ObjectToWorld = *mObjectToWorld;
		record->WorldToObject = *mWorldToObject;
		record->normal = mNormalList[3 * bHitIndex];
		return true;
	}

	return false;
}

void TriangleMesh::Deserialization( tinyxml2::XMLElement* ShapeRootElement )
{
	const char* name = ShapeRootElement->Attribute( "filename" );
	mFilename = new char[strlen( name )];
	strcpy( mFilename , name );

	ParseVector( ShapeRootElement->FirstChildElement( "transform" )->Attribute( "position" ) , &mWorldPos[0] );

	ModelParse( mFilename , mPointList , mNormalList , mTexcoordList , mTriangleList , mVertexCount , mTriangleCount );

	*mObjectToWorld = Translate( Vector3f( mWorldPos ) );
	*mWorldToObject = Inverse( *mObjectToWorld );

	for( int i = 0; i < mTriangleCount; i++ )
	{
		mTriangleList[i].SetTriangleMesh( this );
	}

	for( int i = 0; i < mVertexCount; i++ )
	{
		BBoxLocal.ExpendToInclude( mPointList[i] );
	}

	BBoxWorld = ( *mObjectToWorld )( BBoxLocal );
}


void TriangleMesh::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	{
		pRootElement->SetAttribute( "type" , GetName() );
	}
	
	{
		pRootElement->SetAttribute("filename", mFilename);
	}

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , mWorldPos.x , mWorldPos.y , mWorldPos.z );

		tinyxml2::XMLElement* pTransformElement = xmlDoc.NewElement( "transform" );

		pTransformElement->SetAttribute( "position" , pText );

		pRootElement->InsertEndChild( pTransformElement );

		SAFE_DELETE( pText );
	}
}

int TriangleMesh::GetChildCount() const
{
	return mTriangleCount;
}

Shape* TriangleMesh::GetChild( int index ) const
{
	return &mTriangleList[index];
}