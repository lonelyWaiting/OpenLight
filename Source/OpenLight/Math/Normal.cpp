#include "Utilities/PCH.h"
#include "Vector3.h"
#include "Normal.h"
#include "IO/Log.h"

Normal::Normal()
	: x( 0 )
	, y( 0 )
	, z( 0 )
{

}

Normal::Normal( double x , double y , double z )
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

Normal& Normal::operator = ( const Normal& n )
{
	x = n.x;
	y = n.y;
	z = n.z;

	return ( *this );
}

Normal::operator Vector3f()
{
	return Vector3f( x , y , z );
}

Normal::operator Vector3f() const
{
	return Vector3f( x , y , z );
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

Normal Normal::operator *( double f ) const
{
	return Normal( x * f , y * f , z * f );
}

Normal& Normal::operator *= ( double f )
{
	x *= f;
	y *= f;
	z *= f;

	return ( *this );
}

Normal Normal::operator / ( double f ) const
{
	Assert( f != 0 );
	double Inv = 1.0f / f;

	return Normal( x * Inv , y * Inv , z * Inv );
}

Normal& Normal::operator /= ( double f )
{
	Assert( f != 0 );

	double Inv = 1.0f / f;

	x *= Inv;
	y *= Inv;
	z *= Inv;
	
	return ( *this );
}

double Normal::operator[](int i) const
{ 
	Assert( i >= 0 && i <= 2 );

	return i == 0 ? x : ( i == 1 ? y : z );
}

double& Normal::operator[](int i)
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

double Normal::operator * ( const Vector3f& v )
{
	return ( x * v.x + y * v.y + z * v.z );
}

double Normal::LengthSquared() const
{
	return ( x * x + y * y + z * z );
}

double Normal::Length() const
{
	return sqrt( x * x + y * y + z * z );
}

bool Normal::HasNAN() const
{
	return std::isnan( x ) || std::isnan( y ) || std::isnan( z );
}

double Dot( const Vector3f& v , const Normal& n )
{
	return ( v.x * n.x + v.y * n.y + v.z * n.z );
}

double Dot( const Normal& n , const Vector3f& v )
{
	return ( v.x * n.x + v.y * n.y + v.z * n.z );
}

double AbsDot( const Normal& n , const Vector3f& v )
{
	return fabs( n.x * v.x + n.y * v.y + n.z * v.z );
}

double AbsDot( const Vector3f& v , const Normal& normal )
{
	return fabs( v.x * normal.x + v.y * normal.y + v.z * normal.z );
}

Normal Normalize( const Normal& n )
{
	return n / n.Length();
}