#include "PCH.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/IntersectRecord.h"
#include "Sampler/Sampling.h"
#include "Sphere.h"

Sphere::Sphere()
{
	m_Radius = 0;
}

Sphere::Sphere( const Transform* ObjectToWorld , const Transform* WorldToObject , float radius )
	: Shape( ObjectToWorld , WorldToObject )
	, m_Radius( radius )
{
}

Sphere::~Sphere()
{
}

Bound3f Sphere::ObjectBound() const
{
	return Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );
}

bool Sphere::Intersect( const Ray& r , IntersectRecord* record ) const
{
	// 将光线从世界空间变换到局部空间
	Ray ray = ( *WorldToObject )( r );

	// 计算二次方程的参数
	float A = ray.Direction.x * ray.Direction.x + ray.Direction.y * ray.Direction.y + ray.Direction.z * ray.Direction.z;
	float B = 2 * ray.Origin.x * ray.Direction.x + 2 * ray.Origin.y * ray.Direction.y + 2 * ray.Origin.z * ray.Direction.z;
	float C = ray.Origin.x * ray.Origin.x + ray.Origin.y * ray.Origin.y + ray.Origin.z * ray.Origin.z - m_Radius * m_Radius;

	float t0 , t1;
	if( !Quadtratic( A , B , C , &t0 , &t1 ) )
	{
		// 没有实数根
		return false;
	}

	// 判断相交点，位于光线的合理位置
	if( t0 > ray.MaxT || t1 < ray.MinT )
	{
		return false;
	}

	float t = t0;
	if( t0 < ray.MinT )
	{
		t = t1;
		if( t > ray.MaxT )
		{
			return false;
		}
	}

	record->HitT = t;
	record->ObjectToWorld = *ObjectToWorld;
	record->WorldToObject = *WorldToObject;

	return true;
}

bool Sphere::IntersectP( const Ray& r ) const
{
	Ray ray = ( *WorldToObject )( r );

	// 计算二次方程的参数
	float A = ray.Direction.x * ray.Direction.x + ray.Direction.y * ray.Direction.y + ray.Direction.z * ray.Direction.z;
	float B = 2 * ray.Origin.x * ray.Direction.x + 2 * ray.Origin.y * ray.Direction.y + 2 * ray.Origin.z * ray.Direction.z;
	float C = ray.Origin.x * ray.Origin.x + ray.Origin.y * ray.Origin.y + ray.Origin.z * ray.Origin.z - m_Radius * m_Radius;

	float t0 , t1;
	if( !Quadtratic( A , B , C , &t0 , &t1 ) )
	{
		// 没有实数根
		return false;
	}

	// 判断相交点，位于光线的合理位置
	if( t0 > ray.MaxT || t1 < ray.MinT )
	{
		return false;
	}

	float t = t0;
	if( t0 < ray.MinT )
	{
		t = t1;
		if( t > ray.MaxT )
		{
			return false;
		}
	}

	return true;
}

void Sphere::ParseShape( XMLElement* ShapeRootElement )
{
	XMLElement* PrimitivePosiitonElement = ShapeRootElement->FirstChildElement( "transform" )->FirstChildElement( "position" );

	float PrimitivePosX , PrimitivePosY , PrimitivePosZ;
	PrimitivePosiitonElement->FirstChildElement( "x" )->QueryFloatText( &PrimitivePosX );
	PrimitivePosiitonElement->FirstChildElement( "y" )->QueryFloatText( &PrimitivePosY );
	PrimitivePosiitonElement->FirstChildElement( "z" )->QueryFloatText( &PrimitivePosZ );

	ShapeRootElement->FirstChildElement( "radius" )->QueryFloatText( &m_Radius );

	*ObjectToWorld = Translate( Vector3f( PrimitivePosX , PrimitivePosY , PrimitivePosZ ) );
	*WorldToObject = Inverse( *ObjectToWorld );
}