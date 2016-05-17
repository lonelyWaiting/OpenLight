#pragma once

#include "Utilities/PCH.h"

class RNG
{
public:
	static RNG& Get();

public:
	double GetDouble();

	float  GetFloat();

	int    GetInt();

	int    GetInt( int _min , int _max );
};