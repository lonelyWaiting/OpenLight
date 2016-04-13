#include "PCH.h"
#include "ONB.h"

#define ONB_EPSILON 0.01f

ONB::ONB()
{

}

ONB::ONB( const Vector3f& u , const Vector3f& v , const Vector3f& w )
{
	U = u;
	V = v;
	W = w;
}

void ONB::InitFromU( const Vector3f& u )
{
	Vector3f n( 1.0f , 0.0f , 0.0f );
	Vector3f m( 0.0f , 1.0f , 0.0f );

	U = Normalize( u );
	W = Cross( U , m );
	if( W.Length() < ONB_EPSILON )		// U , m½üºõÆ½ÐÐ
	{
		W = Cross( n , U );
		V = Cross( W , U );
	}
	else
	{
		V = Cross( W , U );
	}

	Normalize( W );
	Normalize( V );
}

void ONB::InitFromV( const Vector3f& v )
{
	Vector3f z( 0.0f , 0.0f , 1.0f );
	Vector3f x( 1.0f , 0.0f , 0.0f );

	V = Normalize( v );
	U = Cross( V , z );
	if( U.Length() < ONB_EPSILON )
	{
		W = Cross( x , V );
		U = Cross( V , W );
	}
	else
	{
		W = Cross( U , V );
	}

	Normalize( W );
	Normalize( U );
}

void ONB::InitFromW( const Vector3f& w )
{
	Vector3f x( 1.0f , 0.0f , 0.0f );
	Vector3f y( 0.0f , 1.0f , 0.0f );

	W = Normalize( w );
	U = Cross( y , W );

	if( U.Length() < ONB_EPSILON )
	{
		V = Cross( x , W );
		U = Cross( V , W );
	}
	else
	{
		V = Cross( W , U );
	}
	
	Normalize( U );
	Normalize( V );
}

void ONB::InitFromUV( const Vector3f& u , const Vector3f& v )
{
	U = Normalize( u );
	W = Normalize( Cross( u , v ) );
	V = Cross( W , U );
}

bool operator == ( const ONB& lhs , const ONB& rhs )
{
	return ( lhs.U == rhs.U ) && ( lhs.V == rhs.V ) && ( lhs.W == rhs.W );
}