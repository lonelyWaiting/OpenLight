#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "FresnelDielectric.h"

FresnelDielectric::FresnelDielectric( float etaI , float etaT )
	: etaI( etaI )
	, etaT( etaT )
{

}

Spectrum FresnelDielectric::Evalute( float cosThetaI ) const
{
	float f = FresnelDielectricIntoDielectric( cosThetaI , etaI , etaT );
	/*double pow = 1.0 / 2.0;

	return Spectrum::FromRGB( std::pow( f , pow ) , std::pow( f , pow ) , std::pow( f , pow ) );*/

	return f;
}