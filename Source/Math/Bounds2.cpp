#include "PCH.h"
#include "Vector2.h"
#include "Point2.h"
#include "Bounds2.h"

Bound2f::Bound2f()
{
	double minNum = std::numeric_limits<double>::lowest();
	double maxNum = std::numeric_limits<double>::max();

	pMin = Point2f( maxNum , maxNum );
	pMax = Point2f( minNum , minNum );
}

Bound2f::Bound2f( const Point2f& p )
	: pMin( p )
	, pMax( p )
{

}

Bound2f::Bound2f( const Point2f& p1 , const Point2f& p2 )
{
	pMin = Point2f( std::min( p1.x , p2.x ) , std::min( p1.y , p2.y ) );
	pMax = Point2f( std::max( p1.x , p2.x ) , std::max( p1.y , p2.y ) );
}

Bound2f::operator Bound2f() const
{
	return Bound2f( pMin , pMax );
}

Vector2f Bound2f::Diagonal() const
{
	return pMax - pMin;
}

bool Bound2f::MaximumExtentIsWidth() const
{
	Vector2f diag = Diagonal();
	if( diag.x > diag.y )
	{
		return true;
	}

	return false;
}

const Point2f& Bound2f::operator[]( int i ) const
{
	assert( i == 0 || i == 1 );
	return ( i == 0 ? pMin : pMax );
}

Point2f& Bound2f::operator[]( int i )
{
	assert( i == 0 || i == 1 );
	return ( i == 0 ? pMin : pMax );
}

bool Bound2f::operator == ( const Bound2f& rhs ) const
{
	return ( rhs.pMin == pMin && rhs.pMax == pMax );
}

bool Bound2f::operator != ( const Bound2f& rhs ) const
{
	return ( rhs.pMin != pMin || rhs.pMax != pMax );
}

Point2f Bound2f::Lerp( const Point2f& t ) const
{
	return Point2f( ::Lerp( t.x , pMin.x , pMax.y ) , ::Lerp( t.y , pMin.y , pMax.y ) );
}

Vector2f Bound2f::Offset( const Point2f& p ) const
{
	Vector2f o = p - pMin;
	if( pMax.x > pMin.x )
	{
		o.x = o.x / ( pMax.x - pMin.x );
	}
	if( pMax.y > pMin.y )
	{
		o.y = o.y / ( pMax.y - pMin.y );
	}

	return o;
}

void Bound2f::BoundingSphere( Point2f* Center , double *Radius ) const
{
	*Center = ( pMin + pMax ) * 0.5f;
	*Radius = Inside( *Center , *this ) ? ( pMax - pMin ).Length() * 0.5f : 0;
}

bool Inside( const Point2f& pt , const Bound2f& b )
{
	return ( pt.x >= b.pMin.x && pt.x <= b.pMax.x && pt.y >= b.pMin.y && pt.y <= b.pMax.y );
}