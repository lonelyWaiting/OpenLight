#pragma once

#include "Fresnel.h"

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric( float etaI , float etaT );

	Spectrum Evalute( float cosThetaI ) const;
private:
	float etaI;
	float etaT;
};