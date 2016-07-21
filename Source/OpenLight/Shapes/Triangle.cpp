#include "Utilities/PCH.h"
#include "Primitive/IntersectRecord.h"
#include "Light/Light.h"
#include "TriangleMesh.h"
#include "tinyxml2.h"
#include "Triangle.h"
#include "Math/Vector2.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/Primitive.h"

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

	const Point3f* points = pMesh->mPointList;

	BBoxLocal.ExpendToInclude( points[index0] );
	BBoxLocal.ExpendToInclude( points[index1] );
	BBoxLocal.ExpendToInclude( points[index2] );

	BBoxWorld = ( *( pMesh->mObjectToWorld ) )( BBoxLocal );
}

bool Triangle::Intersect( Rayf& rayWorld , IntersectRecord* record ) const
{
	// transform ray to local space
	Rayf rayLocal = ( *( pMesh->mWorldToObject ) )( rayWorld );

	const Point3f* points = pMesh->mPointList;

	Point3f p0 = points[index0];
	Point3f p1 = points[index1];
	Point3f p2 = points[index2];

	float ei_minus_hf = ( p0.y - p2.y ) * rayLocal.Direction.z - ( p0.z - p2.z ) * rayLocal.Direction.y;
	float gf_minus_di = ( p0.z - p2.z ) * rayLocal.Direction.x - ( p0.x - p2.x ) * rayLocal.Direction.z;
	float dh_minus_eg = ( p0.x - p2.x ) * rayLocal.Direction.y - ( p0.y - p2.y ) * rayLocal.Direction.x;

	float Determinant = ( p0.x - p1.x ) * ei_minus_hf + ( p0.y - p1.y ) * gf_minus_di + ( p0.z - p1.z ) * dh_minus_eg;

	float beta = ( p0.x - rayLocal.Origin.x ) * ei_minus_hf + ( p0.y - rayLocal.Origin.y ) * gf_minus_di + ( p0.z - rayLocal.Origin.z ) * dh_minus_eg;

	beta /= Determinant;

	if( beta <= 0.0f || beta >= 1.0f )
	{
		return false;
	}

	float ak_minus_jb = ( p0.x - p1.x ) * ( p0.y - rayLocal.Origin.y ) - ( p0.y - p1.y ) * ( p0.x - rayLocal.Origin.x );
	float jc_minus_al = ( p0.z - p1.z ) * ( p0.x - rayLocal.Origin.x ) - ( p0.x - p1.x ) * ( p0.z - rayLocal.Origin.z );
	float bl_minus_kc = ( p0.y - p1.y ) * ( p0.z - rayLocal.Origin.z ) - ( p0.z - p1.z ) * ( p0.y - rayLocal.Origin.y );

	float gamma = rayLocal.Direction.z * ak_minus_jb + rayLocal.Direction.y * jc_minus_al + rayLocal.Direction.x * bl_minus_kc;

	gamma /= Determinant;

	if( gamma <= 0.0f || beta + gamma >= 1.0f )
	{
		return false;
	}

	float t = -( ( p0.z - p2.z ) * ak_minus_jb + ( p0.y - p2.y ) * jc_minus_al + ( p0.x - p2.x ) * bl_minus_kc );

	t /= Determinant;

	if( t >= rayLocal.MinT && t <= rayLocal.MaxT )
	{
		if (pMesh->mTexcoordList)
		{
			Vector2f uv0 = pMesh->mTexcoordList[index0];
			Vector2f uv1 = pMesh->mTexcoordList[index1];
			Vector2f uv2 = pMesh->mTexcoordList[index2];

			float alpha = 1.0 - beta - gamma;

			record->uv = (uv0 * alpha + uv1 * beta + uv2 * gamma) * mPrimitivePtr->GetUVScale();
		}
		
		rayWorld.MaxT         = t;
		record->HitT          = t;
		record->ObjectToWorld = *( pMesh->mObjectToWorld );
		record->WorldToObject = *( pMesh->mWorldToObject );
		record->normal        = ( *mObjectToWorld )( pMesh->mNormalList[index0] );			// 该triangle上的三个顶点
		record->HitPoint      = rayWorld( t );
		record->primitivePtr  = mPrimitivePtr;

		return true;
	}

	return false;
}

float Triangle::Area() const
{
	// |\overrightarrow{a} \times \overrightarrow{b}| = |\overrightarrow{a}| \times |\overrightarrow{b}|sin\theta
	// so Area = \frac{|\overrightarrow{a} \times \overrightarrow{b}|}{2}
	
	// First construct two vector
	Vector3f v1 = pMesh->mPointList[index1] - pMesh->mPointList[index0];
	Vector3f v2 = pMesh->mPointList[index2] - pMesh->mPointList[index0];

	float area = 1.0 / 2.0 * Cross( v1 , v2 ).Length();

	return area;
}

void Triangle::Deserialization( tinyxml2::XMLElement* RootElement )
{ 
	// nothing to do , we only Deserialization obj model , never Deserialization single triangle
	return;
}

Point3f Triangle::Sample( const Point3f& p , LightSample& lightSample , Vector3f& SampleNormal )
{
	// 保证0.0 < u + v < 1.0
	float u = 1.0f - sqrt( lightSample.value[0] );
	float v = lightSample.value[1] * sqrt( lightSample.value[0] );

	// 使用重心坐标
	Point3f SamplePoint = pMesh->mPointList[index0] * u + pMesh->mPointList[index1] * v + pMesh->mPointList[index2] * ( 1.0 - u - v );

	/*SampleNormal = Normal( Cross( pMesh->points[index1] - pMesh->points[index0] , pMesh->points[index2] - pMesh->points[index0] ) );*/
	SampleNormal = pMesh->mNormalList[index0];

	SampleNormal = Normalize( SampleNormal );

	return SamplePoint;
}