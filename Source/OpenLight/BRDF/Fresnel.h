#pragma once

#include "Spectrum/Spectrum.h"

class Fresnel
{
public:
	virtual ~Fresnel();

	virtual Spectrum Evalute( float cosi ) const = 0;
};

Spectrum FresnelDieletricIntoConductor( float CosThetaI , const Spectrum& etaI , const Spectrum& etaT , const Spectrum& k );

float FresnelDielectricIntoDielectric( float CosThetaI , float etaI , float etaT );