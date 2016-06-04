#pragma once

#include "Core/VSerializableObject.h"
#include "Spectrum/Spectrum.h"
#include "Core/VCustomRTTI.h"

class Ray;

class Environment  : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( Environment )

public:
	virtual Spectrum Evalute( Ray& r ) = 0;
};