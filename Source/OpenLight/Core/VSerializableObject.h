#pragma once

#include "Math/Bound3.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

class VSerializableObject
{
public:
	virtual void Deserialization( tinyxml2::XMLElement* RootElement );

	virtual void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );
};