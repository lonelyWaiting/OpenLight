#include "PCH.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/IntersectRecord.h"
#include "Sampler/Sampling.h"
#include "Sphere.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( Sphere , Shape )

Sphere::Sphere()
{
	m_Radius = 0;
}

Sphere::Sphere( Point3f Center , double radius )
	: m_Radius( radius )
	, m_Center( Center )
{
	*ObjectToWorld = Translate( Vector3f( Center ) );
	
	*WorldToObject = Inverse( *ObjectToWorld );

	BBoxLocal = Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}

Sphere::~Sphere()
{
}

bool Sphere::Intersect( Ray& r , IntersectRecord* record ) const
{
	if( BBoxWorld.IntersectP( r ) )
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

		r.MaxT                = t;
		record->primitivePtr  = pPrimitive;
		record->HitT          = t;
		record->ObjectToWorld = *ObjectToWorld;
		record->WorldToObject = *WorldToObject;
		record->normal        = Normal( Normalize( r( t ) - m_Center ) );
		record->Emission      = Emissive;
		record->SurfaceColor  = SurfaceColor;
		record->HitPoint      = r( t );
		return true;
	}
	
	return false;
}

void Sphere::Deserialization( XMLElement* ShapeRootElement )
{
	XMLElement* PrimitivePosiitonElement = ShapeRootElement->FirstChildElement( "transform" )->FirstChildElement( "position" );

	PrimitivePosiitonElement->FirstChildElement( "x" )->QueryDoubleText( &( m_Center.x ) );
	PrimitivePosiitonElement->FirstChildElement( "y" )->QueryDoubleText( &( m_Center.y ) );
	PrimitivePosiitonElement->FirstChildElement( "z" )->QueryDoubleText( &( m_Center.z ) );

	ShapeRootElement->FirstChildElement( "radius" )->QueryDoubleText( &m_Radius );

	// Read Emissive
	double r, g, b;
	ShapeRootElement->FirstChildElement("Emissive")->FirstChildElement("r")->QueryDoubleText(&r);
	ShapeRootElement->FirstChildElement("Emissive")->FirstChildElement("g")->QueryDoubleText(&g);
	ShapeRootElement->FirstChildElement("Emissive")->FirstChildElement("b")->QueryDoubleText(&b);

	Emissive = Spectrum::FromRGB(r, g, b);

	// Read Surface Color
	ShapeRootElement->FirstChildElement( "SurfaceColor" )->FirstChildElement( "r" )->QueryDoubleText( &r );
	ShapeRootElement->FirstChildElement( "SurfaceColor" )->FirstChildElement( "g" )->QueryDoubleText( &g );
	ShapeRootElement->FirstChildElement( "SurfaceColor" )->FirstChildElement( "b" )->QueryDoubleText( &b );
	
	SurfaceColor = Spectrum::FromRGB( r , g , b );

	*ObjectToWorld = Translate( Vector3f( m_Center ) );
	*WorldToObject = Inverse( *ObjectToWorld );

	BBoxLocal = Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}