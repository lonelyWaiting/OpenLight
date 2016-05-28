#include "Utilities/PCH.h"

bool isNanN( const double x )
{
	return std::isnan( x );
}

bool isNanN( const int x )
{
	return false;
}

double Lerp( const double& t , const double& p1 , const double& p2 )
{
	return ( 1 - t ) * p1 + p2 * t;
}

double ToRadians( double degree )
{
	return ( ( degree / 180.0 ) * PI );
}

double ToDegree( double radian )
{
	return ( ( 180.0 / PI ) * radian );
}

void Swap( double& A , double& B )
{
	double C = A;
	A = B;
	B = C;
}

bool Quadtratic( double A , double B , double C , double* t0 , double* t1 )
{
	double delta = B * B - 4 * A * C;

	if( delta < 0 )
	{
		// 不存在实数根
		return false;
	}

	double sqrtDelta = sqrt( delta );

	double InvA = 1.0 / A;

	*t0 = ( -1.0f * B - 1.0 * sqrtDelta ) * 0.5 * InvA;
	*t1 = ( -1.0f * B + sqrtDelta ) * 0.5 * InvA;

	if( *t0 > *t1 )
	{
		//Swap( *t0 , *t1 );
		double temp = *t0;
		*t0 = *t1;
		*t1 = temp;
	}

	return true;
}