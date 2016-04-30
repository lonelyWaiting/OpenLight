#pragma once

#include "Math/Bound3.h"

class VSerializableObject
{
public:
	virtual void Deserialization( XMLElement* RootElement )
	{
	};
};