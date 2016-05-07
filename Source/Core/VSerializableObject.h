#pragma once

#include "Math/Bound3.h"

namespace tinyxml2
{
	class XMLElement;
}

class VSerializableObject
{
public:
	virtual void Deserialization( tinyxml2::XMLElement* RootElement );
};