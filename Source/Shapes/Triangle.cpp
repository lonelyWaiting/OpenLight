#include "PCH.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/IntersectRecord.h"
#include "TriangleMesh.h"
#include "Triangle.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( Triangle , Shape )

Triangle::Triangle()
	: index0( 0 ) 
	, index1( 0 ) 
	, index2( 0 )
{

}

Triangle::~Triangle()
{

}

void Triangle::Set( int _index0 , int _index1 , int _index2 )
{
	index0 = _index0;
	index1 = _index1;
	index2 = _index2;
}

void Triangle::SetTriangleMesh( TriangleMesh* _pMesh )
{
	pMesh = _pMesh;

	const Point3f* points = pMesh->points;

	BBoxLocal.ExpendToInclude( points[index0] );
	BBoxLocal.ExpendToInclude( points[index1] );
	BBoxLocal.ExpendToInclude( points[index2] );

	BBoxWorld = ( *( pMesh->ObjectToWorld ) )( BBoxLocal );
}

bool Triangle::Intersect( Ray& rayWorld , IntersectRecord* record ) const
{
	// transform ray to local space
	Ray rayLocal = ( *( pMesh->WorldToObject ) )( rayWorld );

	const Point3f* points = pMesh->points;

	Point3f p0 = points[index0];
	Point3f p1 = points[index1];
	Point3f p2 = points[index2];

	double ei_minus_hf = ( p0.y - p2.y ) * rayLocal.Direction.z - ( p0.z - p2.z ) * rayLocal.Direction.y;
	double gf_minus_di = ( p0.z - p2.z ) * rayLocal.Direction.x - ( p0.x - p2.x ) * rayLocal.Direction.z;
	double dh_minus_eg = ( p0.x - p2.x ) * rayLocal.Direction.y - ( p0.y - p2.y ) * rayLocal.Direction.x;

	double Determinant = ( p0.x - p1.x ) * ei_minus_hf + ( p0.y - p1.y ) * gf_minus_di + ( p0.z - p1.z ) * dh_minus_eg;

	double beta = ( p0.x - rayLocal.Origin.x ) * ei_minus_hf + ( p0.y - rayLocal.Origin.y ) * gf_minus_di + ( p0.z - rayLocal.Origin.z ) * dh_minus_eg;

	beta /= Determinant;

	if( beta <= 0.0f || beta >= 1.0f )
	{
		return false;
	}

	double ak_minus_jb = ( p0.x - p1.x ) * ( p0.y - rayLocal.Origin.y ) - ( p0.y - p1.y ) * ( p0.x - rayLocal.Origin.x );
	double jc_minus_al = ( p0.z - p1.z ) * ( p0.x - rayLocal.Origin.x ) - ( p0.x - p1.x ) * ( p0.z - rayLocal.Origin.z );
	double bl_minus_kc = ( p0.y - p1.y ) * ( p0.z - rayLocal.Origin.z ) - ( p0.z - p1.z ) * ( p0.y - rayLocal.Origin.y );

	double gamma = rayLocal.Direction.z * ak_minus_jb + rayLocal.Direction.y * jc_minus_al + rayLocal.Direction.x * bl_minus_kc;

	gamma /= Determinant;

	if( gamma <= 0.0f || beta + gamma >= 1.0f )
	{
		return false;
	}

	double t = -( ( p0.z - p2.z ) * ak_minus_jb + ( p0.y - p2.y ) * jc_minus_al + ( p0.x - p2.x ) * bl_minus_kc );

	t /= Determinant;

	if( t >= rayLocal.MinT && t <= rayLocal.MaxT )
	{
		rayWorld.MaxT         = t;
		record->HitT          = t;
		record->ObjectToWorld = *( pMesh->ObjectToWorld );
		record->WorldToObject = *( pMesh->WorldToObject );
		record->normal        = ( *ObjectToWorld )( pMesh->normals[index0] );			// 该triangle上的三个顶点
		record->Emission      = pMesh->Emissive;
		record->SurfaceColor  = pMesh->SurfaceColor;
		record->HitPoint      = rayWorld( t );
		record->primitivePtr  = pMesh->pPrimitive;
		return true;
	}


	return false;
}

void Triangle::Deserialization( XMLElement* RootElement )
{ 
	// nothing to do , i only Deserialization obj model , never Deserialization single triangle
	return;
}