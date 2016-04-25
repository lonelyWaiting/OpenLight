#include "PCH.h"
#include "Fresnel.h"

Fresnel::~Fresnel()
{

}

Spectrum FresnelDieletricIntoConductor( double CosThetaI , const Spectrum& etaI , const Spectrum& etaT , const Spectrum& k )
{
	Clamp( CosThetaI , -1 , 1 );

	Spectrum eta = etaT / etaI;
	Spectrum etaK = k / etaI;

	double cosThetaI2 = CosThetaI * CosThetaI;
	double SinThetaI2 = 1 - cosThetaI2;

	Spectrum eta2 = eta * eta;
	Spectrum etaK2 = etaK * etaK;

	Spectrum t0 = eta2 - etaK2 - SinThetaI2;
	Spectrum a2plusb2 = Sqrt( t0 * t0 + 4 * eta2 * etaK2 );
	Spectrum t1 = a2plusb2 + cosThetaI2;
	Spectrum a = Sqrt( 0.5 * ( a2plusb2 + t0 ) );
	Spectrum t2 = 2 * a * CosThetaI;
	Spectrum Rs = ( t1 - t2 ) / ( t1 + t2 );

	Spectrum t3 = cosThetaI2 * a2plusb2 + SinThetaI2 * SinThetaI2;
	Spectrum t4 = t2 * SinThetaI2;
	Spectrum Rp = Rs * ( t3 - t4 ) / ( t3 + t4 );

	return ( 0.5 * ( Rs + Rp ) );
}

double FresnelDielectricIntoDielectric( double CosThetaI , double etaI , double etaT )
{
	Clamp( CosThetaI , -1 , 1 );

	bool entering = CosThetaI > 0.;
	if( !entering )
	{
		std::swap( etaI , etaT );
		CosThetaI = std::abs( CosThetaI );
	}

	double sinThetaI = std::sqrt( std::max( double( 0 ) , 1 - CosThetaI * CosThetaI ) );
	double sinThetaT = etaI / etaT * sinThetaI;

	// È«ÄÚ·´Éä
	if( sinThetaT >= 1 )
	{
		return 1;
	}

	double CosThetaT = std::sqrt( std::max( double( 0 ) , 1 - sinThetaT * sinThetaT ) );

	double Rparl = ( etaT * CosThetaI - etaI * CosThetaT ) / ( etaT * CosThetaI + etaI * CosThetaT );
	double Rperp = ( etaI * CosThetaI - etaT * CosThetaT ) / ( etaI * CosThetaI + etaT * CosThetaT );

	return ( Rparl * Rparl + Rperp * Rperp ) / 2.0;
}