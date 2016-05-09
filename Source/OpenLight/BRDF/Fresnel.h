#pragma once

#include "Spectrum/Spectrum.h"

class Fresnel
{
public:
	virtual ~Fresnel();

	virtual Spectrum Evalute( double cosi ) const = 0;
};

Spectrum FresnelDieletricIntoConductor( double CosThetaI , const Spectrum& etaI , const Spectrum& etaT , const Spectrum& k );

double FresnelDielectricIntoDielectric( double CosThetaI , double etaI , double etaT );