#include "Utilities/PCH.h"
#include "FresnelDielectric.h"

FresnelDielectric::FresnelDielectric( double etaI , double etaT )
	: etaI( etaI )
	, etaT( etaT )
{

}

Spectrum FresnelDielectric::Evalute( double cosThetaI ) const
{
	return FresnelDielectricIntoDielectric( cosThetaI , etaI , etaT );
}