#include "Utilities/PCH.h"
#include "Point2.h"
#include "Point3.h"
#include "Vector2.h"

Vector2f::Vector2f()
{
	x = y = 0;
}

Vector2f::Vector2f( double _x , double _y )
	: x( _x )
	, y( _y )
{

}

Vector2f::Vector2f( const Point2f &p )
{
	x = p.x;
	y = p.y;
}

Vector2f::Vector2f( const Point3f &p )
{
	x = p.x;
	y = p.y;
}

Vector2f::Vector2f( const Vector2f &rhs )
{
	x = rhs.x;
	y = rhs.y;
}

Vector2f& Vector2f::operator = ( const Vector2f &rhs )
{
	x = rhs.x;
	y = rhs.y;

	return *this;
}

std::ostream& operator << ( std::ostream &os , const Vector2f &rhs )
{
	os << "[" << rhs.x << ", " << rhs.y << "]";
	return os;
}

Vector2f Vector2f::operator + ( const Vector2f& v ) const
{
	return Vector2f( x + v.x , y + v.y );
}

Vector2f& Vector2f::operator += ( const Vector2f& v )
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2f Vector2f::operator - ( const Vector2f &v ) const
{
	return Vector2f( x - v.x , y - v.y );
}

Vector2f& Vector2f::operator -= ( const Vector2f &v )
{
	x -= v.x;
	y -= v.y;

	return *this;
}

bool Vector2f::operator == ( const Vector2f &v ) const
{
	return ( x == v.x && y == v.y );
}

bool Vector2f::operator != ( const Vector2f & v ) const
{
	return ( x != v.x || y != v.y );
}

Vector2f Vector2f::operator* ( double f ) const
{
	assert( !std::isnan( f ) );

	return Vector2f( f * x , f * y );
}

Vector2f& Vector2f::operator *= ( double f )
{
	x *= f;
	y *= f;

	return *this;
}

Vector2f Vector2f::operator / ( double f ) const
{
	assert( f != 0 );
	double inv = ( double )1 / f;
	return Vector2f( x * inv , y * inv );
}

Vector2f& Vector2f::operator /= ( double f )
{
	double inv = ( double )1 / f;

	x *= inv;
	y *= inv;

	return *this;
}

Vector2f Vector2f::operator - () const
{
	return Vector2f( -x , -y );
}

double& Vector2f::operator[] ( int index )
{
	assert( index >= 0 && index < 2 );

	return ( &x )[index];
}

double Vector2f::LengthSq() const
{
	return x * x + y * y;
}

double Vector2f::Length() const
{
	return std::sqrt( LengthSq() );
}

inline Vector2f operator* ( double f , const Vector2f &v )
{
	return v * f;
}

inline double Dot( const Vector2f &v1 , const Vector2f &v2 )
{
	return ( v1.x * v2.x + v1.y * v2.y );
}

inline double AbsDot( const Vector2f &v1 , const Vector2f &v2 )
{
	return std::abs( Dot( v1 , v2 ) );
}

inline Vector2f Normalize( const Vector2f &v )
{
	return v / v.Length();
}

Vector2f Abs( const Vector2f &v )
{
	return Vector2f( std::abs( v.x ) , std::abs( v.y ) );
}