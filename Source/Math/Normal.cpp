#include "PCH.h"
#include "Vector3.h"
#include "Normal.h"
#include "IO/Log.h"

Normal::Normal()
	: x( 0 )
	, y( 0 )
	, z( 0 )
{

}

Normal::Normal( float x , float y , float z )
	: x( x )
	, y( y )
	, z( z )
{

}

Normal::Normal( const Vector3f& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Normal Normal::operator - () const
{
	return Normal( -x , -y , -z );
}

Normal Normal::operator + ( const Normal& n ) const
{
	return Normal( x + n.x , y + n.y , z + n.z );
}

Normal Normal::operator - ( const Normal& n ) const
{
	return Normal( x - n.x , y - n.y , z - n.z );
}

Normal& Normal::operator += ( const Normal& n )
{
	x += n.x;
	y += n.y;
	z += n.z;

	return ( *this );
}

Normal& Normal::operator -= ( const Normal& n )
{
	x -= n.x;
	y -= n.y;
	z -= n.z;

	return ( *this );
}

Normal Normal::operator *( float f ) const
{
	return Normal( x * f , y * f , z * f );
}

Normal& Normal::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;

	return ( *this );
}

Normal Normal::operator / ( float f ) const
{
	Assert( f != 0 );
	float Inv = 1.0f / f;

	return Normal( x * Inv , y * Inv , z * Inv );
}

Normal& Normal::operator /= ( float f )
{
	Assert( f != 0 );

	float Inv = 1.0f / f;

	x *= Inv;
	y *= Inv;
	z *= Inv;
	
	return ( *this );
}

float Normal::operator[](int i) const
{ 
	Assert( i >= 0 && i <= 2 );

	return i == 0 ? x : ( i == 1 ? y : z );
}

float& Normal::operator[](int i)
{ 
	Assert( i >= 0 && i <= 2 );

	return i == 0 ? x : ( i == 1 ? y : z );
}

bool Normal::operator == ( const Normal& rhs ) const
{
	return ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z );
}

bool Normal::operator != ( const Normal& rhs ) const
{
	return !( *this == rhs );
}

float Normal::operator * ( const Vector3f& v )
{
	return ( x * v.x + y * v.y + z * v.z );
}

float Normal::LengthSquared() const
{
	return ( x * x + y * y + z * z );
}

float Normal::Length() const
{
	return sqrtf( x * x + y * y + z * z );
}

bool Normal::HasNAN() const
{
	return std::isnan( x ) || std::isnan( y ) || std::isnan( z );
}

float Dot( const Vector3f& v , const Normal& n )
{
	return ( v.x * n.x + v.y * n.y + v.z * n.z );
}

float Dot( const Normal& n , const Vector3f& v )
{
	return ( v.x * n.x + v.y * n.y + v.z * n.z );
}

float AbsDot( const Normal& n , const Vector3f& v )
{
	return fabs( n.x * v.x + n.y * v.y + n.z * v.z );
}