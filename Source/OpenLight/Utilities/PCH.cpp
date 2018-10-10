#include "Utilities/PCH.h"

bool isNanN( const float x )
{
	return std::isnan( x );
}

bool isNanN( const int x )
{
	return false;
}

float Lerp( const float& t , const float& p1 , const float& p2 )
{
	return ( 1 - t ) * p1 + p2 * t;
}

float ToRadians( float degree )
{
	return ( ( degree / 180.0f ) * PI );
}

float ToDegree( float radian )
{
	return ( ( 180.0f / PI ) * radian );
}

void Swap( float& A , float& B )
{
	float C = A;
	A = B;
	B = C;
}

bool Quadtratic( float A , float B , float C , float* t0 , float* t1 )
{
	float delta = B * B - 4 * A * C;

	if( delta < 0 )
	{
		// 不存在实数根
		return false;
	}

	float sqrtDelta = sqrt( delta );

	float InvA = 1.0f / A;

	*t0 = ( -1.0f * B - 1.0f * sqrtDelta ) * 0.5f * InvA;
	*t1 = ( -1.0f * B + sqrtDelta ) * 0.5f * InvA;

	if( *t0 > *t1 )
	{
		//Swap( *t0 , *t1 );
		float temp = *t0;
		*t0 = *t1;
		*t1 = temp;
	}

	return true;
}

float clamp( float x , float _min , float _max )
{
	return x < _min ? _min : x > _max ? _max : x;
}