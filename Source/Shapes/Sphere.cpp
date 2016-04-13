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

Sphere::Sphere( const Transform* ObjectToWorld , const Transform* WorldToObject , double radius )
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
	double A = ray.Direction.x * ray.Direction.x + ray.Direction.y * ray.Direction.y + ray.Direction.z * ray.Direction.z;
	double B = 2 * ray.Origin.x * ray.Direction.x + 2 * ray.Origin.y * ray.Direction.y + 2 * ray.Origin.z * ray.Direction.z;
	double C = ray.Origin.x * ray.Origin.x + ray.Origin.y * ray.Origin.y + ray.Origin.z * ray.Origin.z - m_Radius * m_Radius;

	double t0 , t1;
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

	double t = t0;
	if( t0 < ray.MinT )
	{
		t = t1;
		if( t > ray.MaxT )
		{
			return false;
		}
	}

	Vector3f v = r( t ) - Point3f( ObjectToWorld->matrix.m30 , ObjectToWorld->matrix.m31 , ObjectToWorld->matrix.m32 );

	if( v.Length() < m_Radius )
	{
		return false;
	}
	

	record->HitT = t;
	record->ObjectToWorld = *ObjectToWorld;
	record->WorldToObject = *WorldToObject;
	Vector3f normal = Normalize(r(t) - Point3f(ObjectToWorld->matrix.m30, ObjectToWorld->matrix.m31, ObjectToWorld->matrix.m32));
	record->normal = Normal(normal.x, normal.y, normal.z);

	return true;
}

bool Sphere::IntersectP( const Ray& r ) const
{
	Ray ray = ( *WorldToObject )( r );

	// 计算二次方程的参数
	double A = ray.Direction.x * ray.Direction.x + ray.Direction.y * ray.Direction.y + ray.Direction.z * ray.Direction.z;
	double B = 2 * ray.Origin.x * ray.Direction.x + 2 * ray.Origin.y * ray.Direction.y + 2 * ray.Origin.z * ray.Direction.z;
	double C = ray.Origin.x * ray.Origin.x + ray.Origin.y * ray.Origin.y + ray.Origin.z * ray.Origin.z - m_Radius * m_Radius;

	double t0 , t1;
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

	double t = t0;
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

	double PrimitivePosX , PrimitivePosY , PrimitivePosZ;
	PrimitivePosiitonElement->FirstChildElement( "x" )->QueryDoubleText( &PrimitivePosX );
	PrimitivePosiitonElement->FirstChildElement( "y" )->QueryDoubleText( &PrimitivePosY );
	PrimitivePosiitonElement->FirstChildElement( "z" )->QueryDoubleText( &PrimitivePosZ );

	ShapeRootElement->FirstChildElement( "radius" )->QueryDoubleText( &m_Radius );

	double r, g, b;
	ShapeRootElement->FirstChildElement("emmisive")->FirstChildElement("r")->QueryDoubleText(&r);
	ShapeRootElement->FirstChildElement("emmisive")->FirstChildElement("g")->QueryDoubleText(&g);
	ShapeRootElement->FirstChildElement("emmisive")->FirstChildElement("b")->QueryDoubleText(&b);

	emmisive = Spectrum::FromRGB(r, g, b);

	*ObjectToWorld = Translate( Vector3f( PrimitivePosX , PrimitivePosY , PrimitivePosZ ) );
	*WorldToObject = Inverse( *ObjectToWorld );
}