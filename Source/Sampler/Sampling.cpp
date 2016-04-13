#include "PCH.h"
#include "Math/Vector2.h"
#include "Sampling.h"

Vector3f UniformSampleHemisphere( const Point2f& u )
{
	double z = u[0];

	double r = std::sqrt( std::max( ( double )9 , ( double )1 - z * z ) );

	double phi = 2 * PI * u[1];

	return Vector3f( r * std::cosf( phi ) , r * std::sinf( phi ) , z );
}

Point2f ConcentricSampleDisk( const Point2f& u )
{
	Point2f SamplePoint = 2 * u - Vector2f( 1.0f , 1.0f );

	if( SamplePoint.x == 0 && SamplePoint.y == 0 )
	{
		return Point2f( 0.0f , 0.0f );
	}

	double r , phi;

	if( SamplePoint.x > -SamplePoint.y )
	{
		if( SamplePoint.x > SamplePoint.y )
		{
			// 区域1
			r = SamplePoint.x;
			phi = PI / 4.0f * SamplePoint.y / SamplePoint.x;
		}
		else
		{
			// 区域2
			r = SamplePoint.y;
			phi = PI / 4.0f * ( 2 - SamplePoint.x / SamplePoint.y );
		}
	}
	else
	{
		if( SamplePoint.x < SamplePoint.y )
		{
			// 区域3
			r = -SamplePoint.x;
			phi = PI / 4.0f * ( 4 + SamplePoint.y / SamplePoint.x );
		}
		else
		{
			r = -SamplePoint.y;
			phi = PI / 4.0f * ( 6 - SamplePoint.x / SamplePoint.y );
		}
	}

	return Point2f( r * std::cosf( phi ) , r * std::sinf( phi ) );
}

Vector3f CosineSampleHemisphere( const Point2f& u )
{
	Point2f SamplePoint = ConcentricSampleDisk( u );

	double z = std::sqrt( std::max( double( 0 ) , 1 - SamplePoint.x * SamplePoint.x - SamplePoint.y * SamplePoint.y ) );

	return Vector3f( SamplePoint.x , SamplePoint.y , z );
}

Vector3f DensitySampleHemisphere( const Point2f& u , double e )
{
	double CosPhi = std::cosf( 2.0f * PI * u.x );

	double SinPhi = std::sinf( 2.0f * PI * u.x );

	double CosTheta = pow( ( 1.0f - u.y ) , 1.0f / ( e + 1 ) );

	double SinTheta = sqrtf( 1.0f - CosTheta * CosTheta );

	return Vector3f( SinTheta * CosPhi , SinTheta * SinPhi , CosTheta );
}

Vector3f UniformSampleSphere( double u1 , double u2 )
{
	double z = 1.0f - 2.0f * u1;

	double r = sqrtf( MAX( 0.0f , 1.0f - z * z ) );

	double phi = 2.0f * PI * u2;

	double x = r * cosf( phi );

	double y = r * sinf( phi );

	return Vector3f( x , y , z );
}

Vector3f UniformSampleCone( double u1 , double u2 , double cosThetaMax , const Vector3f& x , const Vector3f& y , const Vector3f& z )
{
	double CosTheta = Lerp( u1 , cosThetaMax , 1.0f );
	double SinTheta = sqrtf( 1.0f - CosTheta *CosTheta );
	double Phi = u2 * 2.0f * PI;

	return cosf( Phi ) * SinTheta * x + sinf( Phi ) * SinTheta * y + CosTheta * z;
}

double UniformConePDF( double cosThetaMax )
{
	return 1.0f / ( 2.0f * PI * ( 1.0f - cosThetaMax ) );
}

double UniformSpherePDF()
{
	return 1.0f / ( 4.0f * PI );
}

double UniformHemisphere()
{
	return 1.0f / ( 2.0f * PI );
}

Point2f MapUnitSquareToUnitDisk( const Point2f& UnitSquareSamplePoint )
{
	Point2f UnitDiskSamplePoint;

	UnitDiskSamplePoint.x = 2.0f * UnitSquareSamplePoint.x - 1.0f;
	UnitDiskSamplePoint.y = 2.0f * UnitSquareSamplePoint.y - 1.0f;

	double r , phi;

	if( UnitDiskSamplePoint.x > -UnitDiskSamplePoint.y )
	{
		if( UnitDiskSamplePoint.x > UnitDiskSamplePoint.y )
		{
			// 区域1
			r = UnitDiskSamplePoint.x;
			phi = PI / 4.0f * UnitDiskSamplePoint.y / UnitDiskSamplePoint.x;
		}
		else
		{
			// 区域2
			r = UnitDiskSamplePoint.y;
			phi = PI / 4.0f * ( 2 - UnitDiskSamplePoint.x / UnitDiskSamplePoint.y );
		}
	}
	else
	{
		if( UnitDiskSamplePoint.x < UnitDiskSamplePoint.y )
		{
			// 区域3
			r = -UnitDiskSamplePoint.x;
			phi = PI / 4.0f * ( 4 + UnitDiskSamplePoint.y / UnitDiskSamplePoint.x );
		}
		else
		{
			r = -UnitDiskSamplePoint.y;
			phi = PI / 4.0f * ( 6 - UnitDiskSamplePoint.x / UnitDiskSamplePoint.y );
		}
	}

	UnitDiskSamplePoint.x = r * cosf( phi );
	UnitDiskSamplePoint.y = r * sinf( phi );

	return UnitDiskSamplePoint;
}

Point3f MapUnitSquareToHemisphere( const Point2f& UnitSquareSamplePoint , double exp /*= 0*/ )
{
	Point3f HemisphereSamplePoint;

	double CosPhi            = cosf( TWO_PI * UnitSquareSamplePoint.x );
	double SinPhi            = sinf( TWO_PI * UnitSquareSamplePoint.x );

	double CosTheta          = pow( 1.0f - UnitSquareSamplePoint.y , 1.0f / ( exp + 1.0f ) );
	double SinTheta          = sqrtf( 1.0f - CosTheta * CosTheta );

	HemisphereSamplePoint.x = SinTheta * CosPhi;
	HemisphereSamplePoint.y = SinTheta * SinPhi;
	HemisphereSamplePoint.z = CosTheta;

	return HemisphereSamplePoint;
}

Point3f MapUnitSquareToSphere( const Point2f& UnitSquareSamplePoint )
{
	Point3f SphereSamplePoint;

	double r1 = UnitSquareSamplePoint.x;
	double r2 = UnitSquareSamplePoint.y;

	SphereSamplePoint.z = 1.0f - 2.0f * r1;			// z的范围为-1 ， 1
	double r = sqrtf( 1.0f - SphereSamplePoint.z * SphereSamplePoint.z );
	double phi = TWO_PI * r2;						// phi的范围为0 ， 1

	SphereSamplePoint.x = r * cosf( phi );
	SphereSamplePoint.y = r * sinf( phi );

	return SphereSamplePoint;
}