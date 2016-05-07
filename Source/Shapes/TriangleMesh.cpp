#include "Utilities/PCH.h"
#include "Math/Bound3.h"
#include "Math/Ray.h"
#include "Primitive/IntersectRecord.h"
#include "ModelParser.h"
#include "Light/Light.h"
#include "tinyxml2.h"
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

void TriangleMesh::Deserialization( tinyxml2::XMLElement* ShapeRootElement )
{
	const char* name = ShapeRootElement->FirstChildElement( "filename" )->GetText();
	filename = new char[strlen( name )];
	strcpy( filename , name );

	tinyxml2::XMLElement* ShapeTransformElement = ShapeRootElement->FirstChildElement( "transform" );

	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "x" )->QueryDoubleText( &Pos.x );
	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "y" )->QueryDoubleText( &Pos.y );
	ShapeTransformElement->FirstChildElement( "position" )->FirstChildElement( "z" )->QueryDoubleText( &Pos.z );

	*ObjectToWorld = Translate( Vector3f( Pos ) );
	*WorldToObject = Inverse( *ObjectToWorld );

	double r , g , b;

	// read Surface Color Data
	tinyxml2::XMLElement* ShapeSurfaceColorElement = ShapeRootElement->FirstChildElement( "SurfaceColor" );
	ShapeSurfaceColorElement->FirstChildElement( "r" )->QueryDoubleText( &r );
	ShapeSurfaceColorElement->FirstChildElement( "g" )->QueryDoubleText( &g );
	ShapeSurfaceColorElement->FirstChildElement( "b" )->QueryDoubleText( &b );

	SurfaceColor = Spectrum::FromRGB( r , g , b );

	// 解析OBJ模型
	ModelParse( filename , points , normals , triangles , VertexNum , TriangleCount );

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
	pRootElement->SetAttribute( "type" , GetName() );
	pRootElement->SetAttribute( "bCompositeObject" , "true" );

	{
		tinyxml2::XMLElement* pFilenameElement = xmlDoc.NewElement( "filename" );

		pFilenameElement->SetText( filename );

		pRootElement->InsertEndChild( pFilenameElement );
	}

	{
		tinyxml2::XMLElement* pTransformElement = xmlDoc.NewElement( "transform" );

		pRootElement->InsertEndChild( pTransformElement );

		tinyxml2::XMLElement* pPositionElement = xmlDoc.NewElement( "position" );

		pTransformElement->InsertEndChild( pPositionElement );

		tinyxml2::XMLElement* pXElement = xmlDoc.NewElement( "x" );

		pXElement->SetText( Pos.x );

		pPositionElement->InsertEndChild( pXElement );

		tinyxml2::XMLElement* pYElement = xmlDoc.NewElement( "y" );

		pYElement->SetText( Pos.y );

		pPositionElement->InsertEndChild( pYElement );

		tinyxml2::XMLElement* pZElement = xmlDoc.NewElement( "z" );

		pZElement->SetText( Pos.z );

		pPositionElement->InsertEndChild( pZElement );
	}

	{
		tinyxml2::XMLElement* pSurfaceElement = xmlDoc.NewElement( "SurfaceColor" );

		pRootElement->InsertEndChild( pSurfaceElement );

		tinyxml2::XMLElement* pRElement = xmlDoc.NewElement( "r" );

		pRElement->SetText( SurfaceColor[0] );

		pSurfaceElement->InsertEndChild( pRElement );

		tinyxml2::XMLElement* pGElement = xmlDoc.NewElement( "g" );

		pGElement->SetText( SurfaceColor[1] );

		pSurfaceElement->InsertEndChild( pGElement );

		tinyxml2::XMLElement* pBElement = xmlDoc.NewElement( "b" );

		pBElement->SetText( SurfaceColor[2] );

		pSurfaceElement->InsertEndChild( pBElement );
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