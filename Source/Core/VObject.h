#pragma once

#include "Math/Bound3.h"

class VObject
{
public:
	virtual void Deserialization( XMLElement* RootElement ) = 0;
};