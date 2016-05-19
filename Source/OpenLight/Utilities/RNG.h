#pragma once

#include "Utilities/PCH.h"

class RNG
{
public:
	static RNG& Get();

public:
	double GetDouble( double _max = 1.0 , double _min = 0.0 );

	float  GetFloat( float _max = 1.0f , float _min = 0.0f );

	int    GetInt( int _max , int _min = 0 );
};