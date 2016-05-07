#include "Utilities/PCH.h"
#include "Vector3.h"
#include "Vector4.h"

Vector4f::Vector4f() :x( double( 0 ) ) , y( double( 0 ) ) , z( double( 0 ) ) , w( double( 0 ) )
{

}

Vector4f::Vector4f( double a ) : x( a ) , y( a ) , z( a ) , w( a )
{

}

Vector4f::Vector4f( double x , double y , double z , double w ) :x( x ) , y( y ) , z( z ) , w( w )
{

}

Vector4f::Vector4f( const Vector4f& v ) : x( v.x ) , y( v.y ) , z( v.z ) , w( v.w )
{

}

Vector4f::Vector4f( const Vector3f& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 1.0f;
}

bool Vector4f::operator == ( const Vector4f& v ) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4f::operator != ( const Vector4f& v ) const
{
	return ( x != v.x ) || ( y != v.y ) || ( z != v.z ) || ( w != v.w );
}

const Vector4f& Vector4f::operator = ( const Vector4f& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;

	return ( *this );
}

const Vector4f& Vector4f::operator += ( const Vector4f& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

Vector4f Vector4f::operator + ( const Vector4f& v ) const
{
	return Vector4f( x + v.x , y + v.y , z + v.z , w + v.w );
}

const Vector4f& Vector4f::operator -= ( const Vector4f& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return ( *this );
}

Vector4f Vector4f::operator - ( const Vector4f& v ) const
{
	return Vector4f( x - v.x , y - v.y , z - v.z , w - v.w );
}

Vector4f Vector4f::operator - () const
{
	return Vector4f( -x , -y , -z , -w );
}

const Vector4f& Vector4f::operator *= ( const Vector4f& v )
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return ( *this );
}

const Vector4f& Vector4f::operator *= ( double a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return ( *this );
}

const Vector4f& Vector4f::operator /= ( const Vector4f& v )
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return ( *this );
}

const Vector4f& Vector4f::operator /= ( double a )
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;

	return ( *this );
}

Vector4f Vector4f::operator / ( const Vector4f& v ) const
{
	return Vector4f( x / v.x , y / v.y , z / v.z , w / v.w );
}

Vector4f Vector4f::operator / ( double a ) const
{
	return Vector4f( x / a , y / a , z / a , w / a );
}

std::ostream& operator << ( std::ostream os , const Vector4f& rhs )
{
	os << "[ , " << rhs.x << " , " << rhs.y << " , " << rhs.z << " , " << rhs.w << "]";

	return os;
}

double Vector4f::length() const
{
	return sqrt( x * x + y * y + z * z + w * w );
}

double Vector4f::lengthSq() const
{
	return x * x + y * y + z * z + w * w;
}

const Vector4f& Vector4f::normalize()
{
	double l = length();

	if( l != double( 0 ) )
	{
		x /= l;
		y /= l;
		z /= l;
		w /= l;
	}

	return *this;
}

unsigned int Vector4f::dimension()
{
	return 4;
}

inline double Dot( const Vector4f &v1 , const Vector4f &v2 )
{
	return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w );
}

inline double AbsDot( const Vector4f& v1 , const Vector4f& v2 )
{
	return std::abs( Dot( v1 , v2 ) );
}

Vector4f Abs( const Vector4f& v )
{
	return Vector4f( std::abs( v.x ) , std::abs( v.y ) , std::abs( v.z ) , std::abs( v.w ) );
}

Vector4f Permute( const Vector4f &v , int x , int y , int z , int w )
{
	return Vector4f( (&v.x)[x] , ( &v.x )[y] , ( &v.x )[z] , ( &v.x )[w] );
}

inline Vector4f Normalize( const Vector4f& v )
{
	return v / v.length();
}