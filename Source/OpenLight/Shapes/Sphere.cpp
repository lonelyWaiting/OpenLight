#include "Utilities/PCH.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/IntersectRecord.h"
#include "Sampler/Sampling.h"
#include "Light/Light.h"
#include "tinyxml2.h"
#include "Sphere.h"

Sphere::Sphere()
{
	m_Radius = 0;
}

Sphere::Sphere( Point3f Center , double radius )
	: m_Radius( radius )
{
	Pos = Center;

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
		record->normal        = Normal( Normalize( r( t ) - Pos ) );
		record->SurfaceColor  = SurfaceColor;
		record->HitPoint      = r( t );
		return true;
	}
	
	return false;
}

void Sphere::Deserialization( tinyxml2::XMLElement* ShapeRootElement )
{
	tinyxml2::XMLElement* PrimitivePosiitonElement = ShapeRootElement->FirstChildElement( "transform" )->FirstChildElement( "position" );

	PrimitivePosiitonElement->FirstChildElement( "x" )->QueryDoubleText( &( Pos.x ) );
	PrimitivePosiitonElement->FirstChildElement( "y" )->QueryDoubleText( &( Pos.y ) );
	PrimitivePosiitonElement->FirstChildElement( "z" )->QueryDoubleText( &( Pos.z ) );

	ShapeRootElement->FirstChildElement( "radius" )->QueryDoubleText( &m_Radius );

	double r, g, b;

	// Read Surface Color
	ShapeRootElement->FirstChildElement( "SurfaceColor" )->FirstChildElement( "r" )->QueryDoubleText( &r );
	ShapeRootElement->FirstChildElement( "SurfaceColor" )->FirstChildElement( "g" )->QueryDoubleText( &g );
	ShapeRootElement->FirstChildElement( "SurfaceColor" )->FirstChildElement( "b" )->QueryDoubleText( &b );
	
	SurfaceColor = Spectrum::FromRGB( r , g , b );

	*ObjectToWorld = Translate( Vector3f( Pos ) );
	*WorldToObject = Inverse( *ObjectToWorld );

	BBoxLocal = Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}

void Sphere::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );
	pRootElement->SetAttribute( "bCompositeObject" , "false" );

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
		tinyxml2::XMLElement* pRadiusElement = xmlDoc.NewElement( "radius" );

		pRadiusElement->SetText( m_Radius );

		pRootElement->InsertEndChild( pRadiusElement );
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

double Sphere::Area() const
{
	return 4.0 * PI * m_Radius * m_Radius;
}

double Sphere::PDF( const Point3f& p , const Vector3f& wi ) const
{
	// Test whether inside sphere
	if( ( p - Pos ).LengthSq() - m_Radius * m_Radius < 1e4f )
	{
		return Shape::PDF( p , wi );
	}

	double SinThetaMax2 = m_Radius * m_Radius / ( p - Pos ).LengthSq();
	double CosThetaMax = sqrt( MAX( 0.0 , 1.0 - SinThetaMax2 ) );

	return UniformConePDF( CosThetaMax );
}

Point3f Sphere::Sample( const Point3f& p , LightSample& lightSample , Normal& SampleNormal )
{
	Vector3f dirZ = Normalize( Pos - p );

	// 在球面上采样一个点
	if( ( p - Pos ).LengthSq() - m_Radius * m_Radius < 1e4f )
	{
		// 使用默认的采样
		Vector3f SampleDir = UniformSampleHemisphere( Point2f( lightSample.value[0] , lightSample.value[1] ) );

		if( Dot( SampleDir , dirZ ) < 0.0 )
		{
			SampleDir *= -1.0;
		}

		Point3f SamplePoint = Pos + Normalize( dirZ ) * m_Radius;

		// Compute Normal Dir
		SampleNormal = Normalize( Normal( SamplePoint - Pos ) );
		
		return SamplePoint;
	}

	
	Vector3f dirX , dirY;
	CoordinateSystem( dirZ , &dirX , &dirY );

	// 均匀采样cone
	double sinThetaMax2 = m_Radius * m_Radius / ( p - Pos ).LengthSq();
	double cosThetaMax = sqrt( MAX( 0.0 , 1.0 - sinThetaMax2 ) );

	Ray r( p , UniformSampleCone( lightSample.value[0] , lightSample.value[1] , cosThetaMax , dirX , dirY , dirZ ) );

	// Test whether intersect
	double t;
	IntersectRecord record;
	Point3f SamplePoint;
	if( !Intersect( r , &record ) )
	{
		// 必定是切线但被判定为无交点
		t = Dot( Pos - p , r.Direction );
		SamplePoint = r( t );
	}
	else
	{
		SamplePoint = record.HitPoint;
	}
	
	SampleNormal = Normalize( SamplePoint - Pos );

	return SamplePoint;
}