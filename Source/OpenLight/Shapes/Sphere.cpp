#include "Utilities/PCH.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Primitive/IntersectRecord.h"
#include "Sampler/Sampling.h"
#include "Light/Light.h"
#include "tinyxml2.h"
#include "Sphere.h"
#include "Utilities/srString.h"
#include "Primitive/Primitive.h"

Sphere::Sphere()
{
	m_Radius = 0;
}

Sphere::Sphere( Point3f Center , float radius )
	: m_Radius( radius )
{
	mWorldPos = Center;

	*mObjectToWorld = Translate( Vector3f( Center ) );
	
	*mWorldToObject = Inverse( *mObjectToWorld );

	BBoxLocal = Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );

	BBoxWorld = ( *mObjectToWorld )( BBoxLocal );
}

Sphere::~Sphere()
{
}

bool Sphere::Intersect( Rayf& r , IntersectRecord* record ) const
{
	if( BBoxWorld.IntersectP( r ) )
	{
		// 将光线从世界空间变换到局部空间
		Rayf ray = ( *mWorldToObject )( r );

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

		Point3f HitPointInLocalSpace = ray( t );
		if( HitPointInLocalSpace.x == 0.0 && HitPointInLocalSpace.y == 0.0 )
		{
			HitPointInLocalSpace.x = 1e-5f * m_Radius;
		}
		
		float phi = atan2f( HitPointInLocalSpace.y , HitPointInLocalSpace.x );
		if( phi < 0.0 )
		{
			phi += 2.0f * PI;
		}

		float u = phi / TWO_PI;
		float theta = acosf( clamp( HitPointInLocalSpace.z / m_Radius , -1.0f , 1.0f ) );
		float v = theta / PI;

		r.MaxT                = t;
		record->primitivePtr  = mPrimitivePtr;
		record->HitT          = t;
		record->ObjectToWorld = *mObjectToWorld;
		record->WorldToObject = *mWorldToObject;
		record->normal        = Vector3f( Normalize( r( t ) - mWorldPos ) );
		record->HitPoint      = r( t );
		record->uv            = Vector2f( u , v ) * mPrimitivePtr->GetUVScale();

		return true;
	}
	
	return false;
}

void Sphere::Deserialization( tinyxml2::XMLElement* ShapeRootElement )
{
	ShapeRootElement->QueryFloatAttribute( "radius" , &m_Radius );

	ParseVector( std::string( ShapeRootElement->FirstChildElement( "transform" )->Attribute( "position" ) ) , &mWorldPos[0] );

	*mObjectToWorld = Translate( Vector3f( mWorldPos ) );
	*mWorldToObject = Inverse( *mObjectToWorld );

	BBoxLocal = Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );

	BBoxWorld = ( *mObjectToWorld )( BBoxLocal );
}

void Sphere::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	{
		pRootElement->SetAttribute( "type" , GetName() );
	}

	{
		pRootElement->SetAttribute( "radius" , m_Radius );
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

float Sphere::Area() const
{
	return 4.0f * PI * m_Radius * m_Radius;
}

float Sphere::PDF( const Point3f& p , const Vector3f& wi ) const
{
	// Test whether inside sphere
	if( ( p - mWorldPos ).LengthSq() - m_Radius * m_Radius < 1e4f )
	{
		return Shape::PDF( p , wi );
	}

	float SinThetaMax2 = m_Radius * m_Radius / ( p - mWorldPos ).LengthSq();
	float CosThetaMax = sqrt( MAX( 0.0f , 1.0f - SinThetaMax2 ) );

	return UniformConePDF( CosThetaMax );
}

Point3f Sphere::Sample( const Point3f& p , LightSample& lightSample , Vector3f& SampleNormal )
{
	Vector3f dirZ = Normalize( mWorldPos - p );

	// 在球面上采样一个点
	if( ( p - mWorldPos ).LengthSq() - m_Radius * m_Radius < 1e4f )
	{
		// 使用默认的采样
		Vector3f SampleDir = UniformSampleHemisphere( Point2f( lightSample.value[0] , lightSample.value[1] ) );

		if( Dot( SampleDir , dirZ ) < 0.0 )
		{
			SampleDir *= -1.0;
		}

		Point3f SamplePoint = mWorldPos + Normalize( dirZ ) * m_Radius;

		// Compute Normal Dir
		SampleNormal = Normalize( Vector3f( SamplePoint - mWorldPos ) );
		
		return SamplePoint;
	}

	
	Vector3f dirX , dirY;
	CoordinateSystem( dirZ , &dirX , &dirY );

	// 均匀采样cone
	float sinThetaMax2 = m_Radius * m_Radius / ( p - mWorldPos ).LengthSq();
	float cosThetaMax = sqrt( MAX( 0.0f , 1.0f - sinThetaMax2 ) );

	Rayf r( p , UniformSampleCone( lightSample.value[0] , lightSample.value[1] , cosThetaMax , dirX , dirY , dirZ ) );

	// Test whether intersect
	float t;
	IntersectRecord record;
	Point3f SamplePoint;
	if( !Intersect( r , &record ) )
	{
		// 必定是切线但被判定为无交点
		t = Dot( mWorldPos - p , r.Direction );
		SamplePoint = r( t );
	}
	else
	{
		SamplePoint = record.HitPoint;
	}
	
	SampleNormal = Normalize( SamplePoint - mWorldPos );

	return SamplePoint;
}

float Sphere::GetRadius()
{
	return m_Radius;
}

void Sphere::SetRadius( float radius )
{
	m_Radius = radius;

	BBoxLocal = Bound3f( Point3f( -m_Radius , -m_Radius , -m_Radius ) , Point3f( m_Radius , m_Radius , m_Radius ) );

	BBoxWorld = ( *mObjectToWorld )( BBoxLocal );
}