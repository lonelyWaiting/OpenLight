#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "FresnelDielectric.h"

FresnelDielectric::FresnelDielectric( double etaI , double etaT )
	: etaI( etaI )
	, etaT( etaT )
{

}

Spectrum FresnelDielectric::Evalute( double cosThetaI ) const
{
	double f = FresnelDielectricIntoDielectric( cosThetaI , etaI , etaT );
	/*double pow = 1.0 / 2.0;

	return Spectrum::FromRGB( std::pow( f , pow ) , std::pow( f , pow ) , std::pow( f , pow ) );*/

	return f;
}