#pragma once

#include "Fresnel.h"

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric( double etaI , double etaT );

	Spectrum Evalute( double cosThetaI ) const;
private:
	double etaI;
	double etaT;
};