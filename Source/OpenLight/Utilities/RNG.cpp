#include "Utilities/PCH.h"
#include "RNG.h"

RNG& RNG::Get()
{
	static RNG rng;

	return rng;
}

double RNG::GetDouble()
{
	return ( double )rand() / ( double )RAND_MAX;
}

float RNG::GetFloat()
{
	return ( float )rand() / ( float )RAND_MAX;
}

int RNG::GetInt()
{
	return rand();
}

int RNG::GetInt( int _min , int _max )
{
	assert( _max > _min );

	return rand() / ( _max - _min ) + _min;
}