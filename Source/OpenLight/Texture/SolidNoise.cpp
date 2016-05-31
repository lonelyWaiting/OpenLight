#include "SolidNoise.h"
#include "Utilities/RNG.h"

SolidNoise::SolidNoise()
{
	grad[0]  = Vector3f( 1 , 1 , 0 );
	grad[1]  = Vector3f(-1 , 1 , 0 );
	grad[2]  = Vector3f( 1 ,-1 , 0 );
	grad[3]  = Vector3f(-1 ,-1 , 0 );

	grad[4]  = Vector3f( 1 , 0 , 1 );
	grad[5]  = Vector3f(-1 , 0 , 1 );
	grad[6]  = Vector3f( 1 , 0 ,-1 );
	grad[7]  = Vector3f(-1 , 0 ,-1 );

	grad[8]  = Vector3f( 0 , 1 , 1 );
	grad[9]  = Vector3f( 0 ,-1 , 1 );
	grad[10] = Vector3f( 0 , 1 ,-1 );
	grad[11] = Vector3f( 0 ,-1 ,-1 );

	grad[12] = Vector3f( 1 , 1 , 0 );
	grad[13] = Vector3f(-1 , 1 , 0 );
	grad[14] = Vector3f( 0 ,-1 , 1 );
	grad[15] = Vector3f( 0 ,-1 ,-1 );

	for( int i = 0; i < 16; i++ )
	{
		phi[i] = i;
	}

	for( int i = 14; i >= 0; i-- )
	{
		int target = int( RNG::Get().GetDouble() * i );
		int temp = phi[i + 1];
		phi[i + 1] = phi[target];
		phi[target] = temp;
	}
}

double SolidNoise::omega( double t ) const
{
	t = ( t > 0.0 ) ? t : -t;

	return ( -6.0 * t * t * t * t * t + 15.0 * t * t * t * t - 10.0 * t * t * t + 1.0 );
}

Vector3f SolidNoise::gamma( int i , int j , int k ) const
{
	int idx = phi[abs( k ) % 16];
	idx = phi[abs( j + idx ) % 16];
	idx = phi[abs( i + idx ) % 16];
	return grad[idx];
}

double SolidNoise::knot( int i , int j , int k , Vector3f& v ) const
{
	return omega( v[0] ) * omega( v[1] ) * omega( v[2] ) * Dot( gamma( i , j , k ) , v );
}

int SolidNoise::intGamma( int i , int j ) const
{
	int idx;
	idx = phi[abs( j ) % 16];
	idx = phi[abs( i + idx ) % 16];
	return idx;
}

double SolidNoise::turbulence( const Vector3f& p , int depth ) const
{
	double sum = 0.0;
	double weight = 1.0;

	Vector3f ptemp( p );

	sum = fabs( noise( ptemp ) );

	for( int i = 0; i < depth; i++ )
	{
		weight = weight * 2;
		ptemp.x = p.x * weight;
		ptemp.y = p.y * weight;
		ptemp.z = p.z * weight;

		sum += fabs( noise( ptemp ) ) / weight;
	}

	return sum;
}


double SolidNoise::dturbulence( const Vector3f& p , int depth , double d ) const
{
	double sum = 0.0;
	double weight = 1.0;
	Vector3f ptemp( p );

	sum += fabs( noise( ptemp ) ) / 4;

	for( int i = 1; i < depth; i++ )
	{
		weight = weight * d;
		ptemp.x = p.x * weight;
		ptemp.y = p.y * weight;
		ptemp.z = p.z * weight;

		sum += fabs( noise( ptemp ) ) / d;
	}

	return sum;
}

double SolidNoise::noise( const Vector3f& p) const
{
	int fi , fj , fk;

	double fu , fv , fw , sum;

	Vector3f v;

	fi = ( int )( floor( p.x ) );
	fj = ( int )( floor( p.y ) );
	fk = ( int )( floor( p.z ) );
	
	fu = p.x - ( double )fi;
	fv = p.y - ( double )fj;
	fw = p.z - ( double )fk;

	sum = 0.0;

	v = Vector3f( fu , fv , fw );
	sum += knot( fi , fj , fk , v );

	v = Vector3f( fu , fv - 1 , fw );
	sum += knot( fi , fj + 1 , fk , v );

	v = Vector3f( fu , fv , fw - 1 );
	sum += knot( fi + 1 , fj + 1 , fk , v );

	v = Vector3f( fu - 1 , fv , fw - 1 );
	sum += knot( fi + 1 , fj , fk + 1 , v );

	v = Vector3f( fu , fv - 1 , fw - 1 );
	sum += knot( fi , fj + 1 , fk + 1 , v );

	v = Vector3f( fu - 1 , fv - 1 , fw - 1 );
	sum += knot( fi + 1 , fj + 1 , fk + 1 , v );

	return sum;
}