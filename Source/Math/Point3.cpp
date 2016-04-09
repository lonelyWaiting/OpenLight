#include "PCH.h"
#include "Vector3.h"
#include "Point3.h"

Point3f::Point3f()
{
	x = y = z = 0;
}

Point3f::Point3f( float _x , float _y , float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

Point3f::Point3f( const Point3f &p )
{
	x = p.x;
	y = p.y;
	z = p.z;
}

Point3f& Point3f::operator = ( const Point3f& p )
{
	x = p.x;
	y = p.y;
	z = p.z;

	return *this;
}

Point3f Point3f::operator + ( const Vector3f& v ) const
{
	return Point3f( x + v.x , y + v.y , z + v.z );
}

Point3f& Point3f::operator += ( const Vector3f& v )
{
	x += v.x;
	y += v.y;
	z += v.z;

	return ( *this );
}

Vector3f Point3f::operator - ( const Point3f& p ) const
{
	return Vector3f( x - p.x , y - p.y , z - p.z );
}

Point3f Point3f::operator - ( const Vector3f &v ) const
{
	return Point3f( x - v.x , y - v.y , z - v.z );
}

Point3f& Point3f::operator -= ( const Vector3f &p )
{
	x -= p.x;
	y -= p.y;
	z -= p.z;

	return ( *this );
}

Point3f Point3f::operator + ( const Point3f &p ) const
{
	return Point3f( x + p.x , y + p.y , z + p.z );
}

Point3f Point3f::operator* ( float f ) const
{
	return Point3f( f * x , f * y , f * z );
}

Point3f& Point3f::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

Point3f Point3f::operator / ( float f ) const
{
	assert( f != 0 );

	float inv = ( float )1 / f;

	return Point3f( x * inv , y * inv , z * inv );
}

Point3f& Point3f::operator /= ( float f )
{
	assert( f != 0 );

	float inv = 1.0f / f;

	x *= inv;
	y *= inv;
	z *= inv;

	return ( *this );
}

float Point3f::operator[] ( int index ) const
{
	assert( index >= 1 && index <= 2 );

	if( index == 0 )	return x;

	if( index == 1 )	return y;

	return z;
}

float& Point3f::operator [] ( int i )
{
	assert( i >= 0 && i <= 2 );

	if( i == 0 )	return x;

	if( i == 1 )	return y;

	return z;
}

bool Point3f::operator == ( const Point3f& p ) const
{
	return ( x == p.x && y == p.y && z == p.z );
}

bool Point3f::operator != ( const Point3f& p ) const
{
	return ( x != p.x || y != p.y || z != p.z );
}

std::ostream & operator<< ( std::ostream & os , const Point3f& p )
{
	os << "[" << p.x << ", " << p.y << ", " << p.z << "]";

	return os;
}

inline float Distance( const Point3f &p1 , const Point3f &p2 )
{
	return ( p1 - p2 ).Length();
}

inline float DistanceSq( const Point3f &p1 , const Point3f &p2 )
{
	return ( p1 - p2 ).LengthSq();
}

inline Point3f operator*( float f , const Point3f &p )
{
	return p * f;
}

Point3f Lerp( float t , const Point3f &p0 , const Point3f& p1 )
{
	return ( 1 - t ) * p0 + t * p1;
}

Point3f Min( const Point3f &p1 , const Point3f &p2 )
{
	return Point3f( std::min( p1.x , p2.x ) , std::min( p1.y , p2.y ) , std::min( p1.z , p2.z ) );
}

Point3f Max( const Point3f &p1 , const Point3f &p2 )
{
	return Point3f( std::max( p1.x , p2.x ) , std::max( p1.y , p1.z ) , std::max( p1.z , p2.z ) );
}

// 向下取整
Point3f Floor( const Point3f &p )
{
	return Point3f( std::floor( p.x ) , std::floor( p.y ) , std::floor( p.z ) );
}

Point3f Ceil( const Point3f &p )
{
	return Point3f( std::ceil( p.x ) , std::ceil( p.y ) , std::ceil( p.z ) );
}

Point3f Abs( const Point3f& p )
{
	return Point3f( std::abs( p.x ) , std::abs( p.y ) , std::abs( p.z ) );
}

Point3f Permute( const Point3f &p , int x , int y , int z )
{
	return Point3f( p.x , p.y , p.z );
}