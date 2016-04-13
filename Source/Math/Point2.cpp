#include "PCH.h"
#include "Vector2.h"
#include "Point2.h"

Point2f::Point2f()
{
	x = y = 0;
}

Point2f::Point2f( double _x , double _y )
{
	x = _x;
	y = _y;
}

Point2f::Point2f( const Point2f &p )
{
	x = p.x;
	y = p.y;
}

Point2f::Point2f( const Vector2f &p )
{
	x = p.x;
	y = p.y;
}

Point2f& Point2f::operator = ( const Point2f &p )
{
	x = p.x;
	y = p.y;

	return ( *this );
}

std::ostream & operator << ( std::ostream & os , const Point2f &p )
{
	os << "[" << p.x << ", " << p.y << "]";

	return os;
}

Point2f Point2f::operator + ( const Vector2f &v ) const
{
	return Point2f( x + v.x , y + v.y );
}

Point2f& Point2f::operator += ( const Vector2f &v )
{
	x += v.x;
	y += v.y;

	return *this;
}

Point2f Point2f::operator + ( const Point2f &p ) const
{
	return Point2f( x + p.x , y + p.y );
}

Vector2f Point2f::operator - ( const Point2f & p ) const
{
	return Vector2f( x - p.x , y - p.y );
}

Point2f Point2f::operator - ( const Vector2f &v ) const
{
	return Point2f( x - v.x , y - v.y );
}

Point2f Point2f::operator - () const
{
	return Point2f( -x , -y );
}

Point2f& Point2f::operator -= ( const Vector2f &v )
{
	x -= v.x;
	y -= v.y;

	return ( *this );
}

Point2f Point2f::operator * ( double f ) const
{
	return Point2f( f * x , f * y );
}

Point2f& Point2f::operator *= ( double f )
{
	x *= f;
	y *= f;

	return ( *this );
}

Point2f Point2f::operator / ( double f ) const
{
	assert( f != 0 );

	double inv = ( double )1 / f;

	return Point2f( inv * x , inv * y );
}

Point2f Point2f::operator /= ( double f )
{
	double inv = ( double )1 / f;

	x *= inv;
	y *= inv;

	return *this;
}

double Point2f::operator[] ( int index ) const
{
	assert( index >= 0 && index <= 1 );

	if( index == 0 )	return x;
	return y;
}

double& Point2f::operator[] ( int index )
{
	assert( index >= 0 && index <= 1 );

	if( index == 0 )	return x;
	return y;
}

bool Point2f::operator == ( const Point2f &p ) const
{
	return ( x == p.x && y == p.y );
}

bool Point2f::operator != ( const Point2f &p ) const
{
	return ( x != p.x || y != p.y );
}

inline double Distance( const Point2f& p1 , const Point2f &p2 )
{
	return ( p1 - p2 ).Length();
}

inline double DistanceSq( const Point2f &p1 , const Point2f &p2 )
{
	return ( p1 - p2 ).LengthSq();
}

inline Point2f operator* ( double f , const Point2f &p )
{
	return p * f;
}

Point2f Floor( const Point2f &p )
{
	return Point2f( std::floor( p.x ) , std::floor( p.y ) );
}

Point2f Ceil( const Point2f &p )
{
	return Point2f( std::ceil( p.x ) , std::ceil( p.y ) );
}

Point2f Lerp( double t , const Point2f &p0 , const Point2f &p1 )
{
	return ( 1 - t ) * p0 + t * p1;
}

Point2f Min( const Point2f &p0 , const Point2f &p1 )
{
	return Point2f( std::min( p0.x , p1.x ) , std::min( p0.y , p1.y ) );
}

Point2f Max( const Point2f &p0 , const Point2f &p1 )
{
	return Point2f( std::max( p0.x , p1.x ) , std::max( p0.y , p1.y ) );
}