#pragma once

#include "Fresnel.h"

// 导体
class FresnelConductor : public Fresnel
{
public:
	Spectrum Evalute( double cosThetaI ) const;

	FresnelConductor( const Spectrum& etaI , const Spectrum& etaT , const Spectrum& k );

private:
	Spectrum etaI;		// 入射媒介的折射率
	Spectrum etaT;		// 折射媒介的折射率
	Spectrum k;			// 吸收系数
};