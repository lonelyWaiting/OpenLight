#pragma once

#include "Core/VSerializableObject.h"
#include "Spectrum/Spectrum.h"
#include "Core/VCustomRTTI.h"

class Environment  : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( Environment )

public:
	virtual Spectrum Evalute( Rayf& r ) = 0;
};