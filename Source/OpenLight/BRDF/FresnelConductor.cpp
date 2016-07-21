#include "Utilities/PCH.h"
#include "FresnelConductor.h"

FresnelConductor::FresnelConductor( const Spectrum& etaI , const Spectrum& etaT , const Spectrum& k )
	: etaI( etaI )
	, etaT( etaT )
	, k( k )
{

}

Spectrum FresnelConductor::Evalute( float cosThetaI ) const
{
	float AbsCosThetaI = std::abs( cosThetaI );

	return FresnelDieletricIntoConductor( AbsCosThetaI , etaI , etaT , k );
}