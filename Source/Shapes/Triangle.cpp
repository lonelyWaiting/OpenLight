#include "PCH.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/IntersectRecord.h"
#include "Triangle.h"

Triangle::Triangle()
	:index0( 0 ) , index1( 0 ) , index2( 0 )
{

}

Triangle::Triangle( int _index0 , int _index1 , int _index2 )
	: index0( _index0 ) , index1( _index1 ) , index2( _index2 )
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

/*
	传入的Ray位于Object Space
*/
bool Triangle::Intersect( const Ray& ray , const Point3f* points , IntersectRecord* record ) const
{
	Point3f p0 = points[index0];
	Point3f p1 = points[index1];
	Point3f p2 = points[index2];

	double ei_minus_hf = ( p0.y - p2.y ) * ray.Direction.z - ( p0.z - p2.z ) * ray.Direction.y;
	double gf_minus_di = ( p0.z - p2.z ) * ray.Direction.x - ( p0.x - p2.x ) * ray.Direction.z;
	double dh_minus_eg = ( p0.x - p2.x ) * ray.Direction.y - ( p0.y - p2.y ) * ray.Direction.x;

	double Determinant = ( p0.x - p1.x ) * ei_minus_hf + ( p0.y - p1.y ) * gf_minus_di + ( p0.z - p1.z ) * dh_minus_eg;

	double beta = ( p0.x - ray.Origin.x ) * ei_minus_hf + ( p0.y - ray.Origin.y ) * gf_minus_di + ( p0.z - ray.Origin.z ) * dh_minus_eg;

	beta /= Determinant;

	if( beta <= 0.0f || beta >= 1.0f )
	{
		return false;
	}

	double ak_minus_jb = ( p0.x - p1.x ) * ( p0.y - ray.Origin.y ) - ( p0.y - p1.y ) * ( p0.x - ray.Origin.x );
	double jc_minus_al = ( p0.z - p1.z ) * ( p0.x - ray.Origin.x ) - ( p0.x - p1.x ) * ( p0.z - ray.Origin.z );
	double bl_minus_kc = ( p0.y - p1.y ) * ( p0.z - ray.Origin.z ) - ( p0.z - p1.z ) * ( p0.y - ray.Origin.y );

	double gamma = ray.Direction.z * ak_minus_jb + ray.Direction.y * jc_minus_al + ray.Direction.x * bl_minus_kc;

	gamma /= Determinant;

	if( gamma <= 0.0f || beta + gamma >= 1.0f )
	{
		return false;
	}

	double t = -( ( p0.z - p2.z ) * ak_minus_jb + ( p0.y - p2.y ) * jc_minus_al + ( p0.x - p2.x ) * bl_minus_kc );

	t /= Determinant;

	if( t >= ray.MinT && t <= ray.MaxT )
	{
		record->HitT = t;
		return true;
	}

	return false;
}

/*
	Ray位于Object Space
*/
bool Triangle::IntersectP( const Ray& ray , const Point3f* points ) const
{
	Point3f p0 = points[index0];
	Point3f p1 = points[index1];
	Point3f p2 = points[index2];

	double ei_minus_hf = ( p0.y - p2.y ) * ray.Direction.z - ( p0.z - p2.z ) * ray.Direction.y;
	double gf_minus_di = ( p0.z - p2.z ) * ray.Direction.x - ( p0.x - p2.x ) * ray.Direction.z;
	double dh_minus_eg = ( p0.x - p2.x ) * ray.Direction.y - ( p0.y - p2.y ) * ray.Direction.x;

	double Determinant = ( p0.x - p1.x ) * ei_minus_hf + ( p0.y - p1.y ) * gf_minus_di + ( p0.z - p1.z ) * dh_minus_eg;

	double beta = ( p0.x - ray.Origin.x ) * ei_minus_hf + ( p0.y - ray.Origin.y ) * gf_minus_di + ( p0.z - ray.Origin.z ) * dh_minus_eg;

	beta /= Determinant;

	if( beta <= 0.0f || beta >= 1.0f )
	{
		return false;
	}

	double ak_minus_jb = ( p0.x - p1.x ) * ( p0.y - ray.Origin.y ) - ( p0.y - p1.y ) * ( p0.x - ray.Origin.x );
	double jc_minus_al = ( p0.z - p1.z ) * ( p0.x - ray.Origin.x ) - ( p0.x - p1.x ) * ( p0.z - ray.Origin.z );
	double bl_minus_kc = ( p0.y - p1.y ) * ( p0.z - ray.Origin.z ) - ( p0.z - p1.z ) * ( p0.y - ray.Origin.y );

	double gamma = ray.Direction.z * ak_minus_jb + ray.Direction.y * jc_minus_al + ray.Direction.x * bl_minus_kc;

	gamma /= Determinant;

	if( gamma <= 0.0f || beta + gamma >= 1.0f )
	{
		return false;
	}

	double t = -( ( p0.z - p2.z ) * ak_minus_jb + ( p0.y - p2.y ) * jc_minus_al + ( p0.x - p2.x ) * bl_minus_kc );

	t /= Determinant;

	if( t >= ray.MinT && t <= ray.MaxT )
	{
		return true;
	}

	return false;
}