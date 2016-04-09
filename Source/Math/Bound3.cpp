#include "PCH.h"
#include "Ray.h"
#include "Vector3.h"
#include "Point3.h"
#include "Bound3.h"

Bound3f::Bound3f()
{
	float MinNumber = std::numeric_limits<float>::lowest();
	float MaxNumber = std::numeric_limits<float>::max();

	pMin = Point3f( MaxNumber , MaxNumber , MaxNumber );
	pMax = Point3f( MinNumber , MinNumber , MinNumber );
}

Bound3f::Bound3f( const Point3f& p )
	:pMin( p )
	, pMax( p )
{

}

Bound3f::Bound3f( const Point3f& p1 , const Point3f& p2 )
	:pMin( std::min( p1.x , p2.x ) , std::min( p1.y , p2.y ) , std::min( p1.z , p2.z ) )
	, pMax( std::max( p1.x , p2.x ) , std::max( p1.y , p2.y ) , std::max( p1.z , p2.z ) )
{

}

const Point3f& Bound3f::operator[]( int i ) const
{
	assert( i == 0 || i == 1 );

	return i == 0 ? pMin : pMax;
}

Point3f& Bound3f::operator[]( int i )
{
	assert( i == 0 || i == 1 );

	return i == 0 ? pMin : pMax;
}

Point3f Bound3f::Corner( int corner ) const
{
	assert( corner >= 0 && corner < 8 );

	return Point3f( ( *this )[( corner & 1 )].x , ( *this )[( corner & 2 ) ? 1 : 0].y , ( *this )[( corner & 4 ) ? 1 : 0].z );
}

Vector3f Bound3f::Diagonal() const
{
	return pMax - pMin;
}

float Bound3f::SurfaceArea() const
{
	Vector3f d = Diagonal();

	return 2 * ( d.x * d.y + d.x * d.z + d.y * d.z );
}

Vector3f Bound3f::Offset( const Point3f& p ) const
{
	Vector3f o = p - pMin;
	if( pMax.x > pMin.x )	o.x /= pMax.x - pMin.x;
	if( pMax.y > pMin.y )	o.y /= pMax.y - pMin.y;
	if( pMax.z > pMin.z )	o.z /= pMax.z - pMin.z;

	return o;
}

void Bound3f::BoundingSphere( Point3f* center , float* radius ) const
{
	*center = ( pMin + pMax ) * 0.5;
	*radius = Inside( *center , *this ) ? ( pMax - pMin ).Length() / 2 : 0;
}

void Bound3f::ExpendToInclude( const Point3f& p )
{
	pMin.x = std::min( pMin.x , p.x );
	pMin.y = std::min( pMin.y , p.y );
	pMin.z = std::min( pMin.z , p.z );

	pMax.x = std::max( pMax.x , p.x );
	pMax.y = std::max( pMax.y , p.y );
	pMax.z = std::max( pMax.z , p.z );
}

bool Inside( const Point3f& pt , const Bound3f& b )
{
	return ( pt.x >= b.pMin.x && pt.x <= b.pMax.x && pt.y >= b.pMin.y && pt.y <= b.pMax.y && pt.z >= b.pMin.z && pt.z <= b.pMax.z );
}

bool Bound3f::IntersectP( const Ray& ray , float* hitt0 /* = nullptr  */ , float* hitt1 /* = nullptr */ ) const
{
	float t0 = 0 , t1 = ray.MaxT;

	for( int i = 0; i < 3; i++ )
	{
		float invRayDir = 1 / ray.Direction[i];

		float tNear = ( pMin[i] - ray.Origin[i] ) * invRayDir;
		float tFar = ( pMax[i] - ray.Origin[i] ) * invRayDir;

		if( tNear > tFar )
		{
			std::swap( tNear , tFar );
		}

		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;

		if( t0 > t1 )
		{
			return false;
		}
	}

	if( hitt0 )
	{
		*hitt0 = t0;
	}

	if( hitt1 )
	{
		*hitt1 = t1;
	}

	return true;
}

bool Bound3f::IntersectP( const Ray & ray , const Vector3f & invDir , const int DirectionIsNegative[3] )
{
	return false;
}