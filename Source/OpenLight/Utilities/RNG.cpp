#include "Utilities/PCH.h"
#include "RNG.h"

RNG& RNG::Get()
{
	static RNG rng;

	return rng;
}

double RNG::GetDouble( double _max /*= 1.0*/ , double _min /*= 0.0*/ )
{
	return ( double )rand() / ( double )RAND_MAX * ( _max - _min ) + _min;
}

float RNG::GetFloat( float _max /*= 1.0f*/ , float _min /*= 0.0f*/ )
{
	return ( float )rand() / ( float )RAND_MAX * ( _max - _min ) + _min;
}

int RNG::GetInt( int _max , int _min /*= 0*/ )
{
	assert( _max > _min );

	return rand() % ( _max - _min ) + _min;
}