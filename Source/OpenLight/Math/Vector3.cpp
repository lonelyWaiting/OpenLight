#include "Utilities/PCH.h"
#include "Point3.h"
#include "Normal.h"
#include "Vector3.h"

Vector3f::Vector3f() : x( 0 ) , y( 0 ) , z( 0 )
{

}

Vector3f::Vector3f( double value ) : x( value ) , y( value ) , z( value )
{

}

Vector3f::Vector3f( double _x , double _y , double _z ) : x( _x ) , y( _y ) , z( _z )
{

}

Vector3f::Vector3f( const Vector3f& rhs ) : x( rhs.x ) , y( rhs.y ) , z( rhs.z )
{

}

Vector3f::Vector3f( const Point3f &p )
{
	x = p.x;
	y = p.y;
	z = p.z;
}

Vector3f::operator Normal()
{
	return Normal( x , y , z );
}

Vector3f& Vector3f::operator = ( const Vector3f& rhs )
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

Vector3f Vector3f::operator + ( const Vector3f& rhs ) const
{
	return Vector3f( x + rhs.x , y + rhs.y , z + rhs.z );
}

Vector3f& Vector3f::operator += ( const Vector3f& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vector3f Vector3f::operator - ( const Vector3f& rhs ) const
{
	return Vector3f( x - rhs.x , y - rhs.y , z - rhs.z );
}

Vector3f& Vector3f::operator -= ( const Vector3f& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Vector3f Vector3f::operator * ( double scale ) const
{
	return Vector3f( x * scale , y * scale , z * scale );
}

Vector3f& Vector3f::operator *= ( double scale )
{
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}

double Vector3f::operator * ( const Vector3f &rhs ) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector3f Vector3f::operator -() const
{
	return Vector3f( -x , -y , -z );
}

Vector3f Vector3f::operator / ( double f ) const
{
	assert( f != 0 );

	double inv = ( double )1 / f;

	return Vector3f( x * inv , y * inv , z * inv );
}

Vector3f& Vector3f::operator /= ( double f )
{
	assert( f != 0 );

	double inv = ( double )1 / f;

	x *= inv;
	y *= inv;
	z *= inv;

	return *this;
}

double& Vector3f::operator [] ( int index )
{
	assert( index >= 0 && index < 3 );

	if( index == 0 ) return x;

	if( index == 1 ) return y;

	return z;
}

double Vector3f::operator[] ( int index ) const
{
	assert( index >= 0 && index < 3 );

	if( index == 0 )	return x;

	if( index == 1 )	return y;

	return z;
}

bool Vector3f::operator == ( const Vector3f& rhs ) const
{
	return ( x == rhs.x && y == rhs.y && z == rhs.z );
}

bool Vector3f::operator != ( const Vector3f& rhs ) const
{
	return ( x != rhs.x || y != rhs.y || z != rhs.z );
}

double Vector3f::LengthSq() const
{
	return ( x * x + y * y + z * z );
}

double Vector3f::Length() const
{
	return std::sqrt( LengthSq() );
}

Vector3f operator* ( double s , const Vector3f &v )
{
	return v * s;
}

Vector3f Abs( const Vector3f &v )
{
	return Vector3f( std::abs( v.x ) , std::abs( v.y ) , std::abs( v.z ) );
}

double Dot( const Vector3f &v1 , const Vector3f &v2 )
{
	return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z );
}

double AbsDot( const Vector3f & v1 , const Vector3f& v2 )
{
	return std::abs( Dot( v1 , v2 ) );
}

Vector3f Cross( const Vector3f &v1 , const Vector3f &v2 )
{
	double x1 = v1.x , y1 = v1.y , z1 = v1.z;
	double x2 = v2.x , y2 = v2.y , z2 = v2.z;

	return Vector3f( y1 * z2 - z1 * y2 , z1 * x2 - x1 * z2 , x1 * y2 - y1 * x2 );
}

Vector3f Normalize( const Vector3f& v )
{
	return v / v.Length();
}

double MinComponent( const Vector3f &v )
{
	return std::min( v.x , std::min( v.y , v.z ) );
}

double MaxCompoent( const Vector3f &v )
{
	return std::max( v.x , std::max( v.y , v.z ) );
}

Vector3f Max( const Vector3f &p1 , const Vector3f &p2 )
{
	return Vector3f( std::max( p1.x , p2.x ) , std::max( p1.y , p2.y ) , std::max( p1.z , p2.z ) );
}

Vector3f Min( const Vector3f &p1 , const Vector3f &p2 )
{
	return Vector3f( std::min( p1.x , p2.x ) , std::min( p1.y , p2.y ) , std::min( p1.z , p2.z ) );
}

Vector3f Permute( const Vector3f &v , int x , int y , int z )
{
	return Vector3f( v[x] , v[y] , v[z] );
}

void CoordinateSystem( Vector3f &v1 , Vector3f *v2 , Vector3f *v3 )
{
	if( std::abs( v1.x ) > std::abs( v1.y ) )
	{
		*v2 = Vector3f( -v1.z , 0 , v1.x ) / std::sqrt( v1.x * v1.x + v1.z * v1.z );
	}
	else
	{
		*v2 = Vector3f( 0 , v1.z , -v1.y ) / std::sqrt( v1.y * v1.y + v1.z + v1.z );
	}

	*v3 = Cross( v1 , *v2 );
}

int MaxLengthComponent(const Vector3f& v)
{
	double absX = std::abs(v.x);
	double absY = std::abs(v.y);
	double absZ = std::abs(v.z);

	if (absX > absY && absX > absZ)
	{
		return 0;
	}
	else if (absY > absZ)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}