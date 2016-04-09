#pragma once

#include "Utilities/PCH.h"
#include "IO/Log.h"

static const int SampledLambdaStart = 400;
static const int SampledLambdaEnd = 700;
static const int NSpectralSamples = 60;

static const int CIESamplesNum = 471;
// 光谱分布函数在波长为360~830时的值
extern const float CIE_X[CIESamplesNum];
extern const float CIE_Y[CIESamplesNum];
extern const float CIE_Z[CIESamplesNum];

extern const float CIE_lambda[CIESamplesNum];

static const int CIE_Y_Integral = 106.856895;

enum class SpectrumType
{
	Reflectance , 
	Illuminant
};

inline void XYZToRGB( const float xyz[3] , float rgb[3] )
{
	rgb[0] = 3.240479f  * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
	rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
	rgb[2] = 0.055648f  * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

inline void RGBToXYZ( const float rgb[3] , float xyz[3] )
{
	xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
	xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
	xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}

// 判断是否是有序的
extern bool IsOrdered( const float* lambda , int n );

extern bool SortSpectrumSamplePoint( float* lambda , float* values , int n );

// lambda为采样点数组，里面记录着各个采样点的波长
// values对应于lambda，记录着各个采样点波长对应的光谱分布函数的值
// n表示lambda的size，也是values的size
// 当前波长
extern float InterpolateSpectrumSamples( const float* lambda , const float* values , int n , float l );

template<int NSpectrumSamples>
class CoefficientSpectrum
{
public:
	CoefficientSpectrum( float v = 0.0f )
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] = v;
		}
		Assert( !HasNAN() );		// 确信没有NAN
	}

	CoefficientSpectrum( const CoefficientSpectrum& rhs )
	{
		Assert( !rhs.HasNAN() );
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] = rhs.c[i];
		}
	}
	
	CoefficientSpectrum& operator = ( const CoefficientSpectrum& rhs )
	{
		Assert( !rhs.HasNAN() );

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] = rhs.c[i];
		}

		return ( *this );
	}

	bool HasNAN() const
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			if( isnan( c[i] ) )		// 该函数位于cmath
			{
				return true;
			}
		}

		return false;
	}

public:
	// operator overload
	CoefficientSpectrum& operator += ( const CoefficientSpectrum& rhs )
	{
		Assert( !rhs.HasNAN() );

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] += rhs.c[i];
		}

		return ( *this );
	}

	CoefficientSpectrum operator + ( const CoefficientSpectrum& rhs ) const
	{
		CoefficientSpectrum result;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] = c[i] + rhs.c[i];
		}

		return result;
	}

	CoefficientSpectrum operator - ( const CoefficientSpectrum& rhs ) const
	{
		Assert( !rhs.HasNAN() );

		CoefficientSpectrum result;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] = c[i] - rhs.c[i];
		}

		return result;
	}

	CoefficientSpectrum operator * ( const CoefficientSpectrum& rhs ) const
	{
		Assert( !rhs.HasNAN() );

		CoefficientSpectrum result;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] = c[i] * rhs.c[i];
		}

		return result;
	}

	CoefficientSpectrum& operator *= ( const CoefficientSpectrum& rhs )
	{
		Assert( !rhs.HasNAN() );

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] *= rhs.c[i];
		}

		return ( *this );
	}

	CoefficientSpectrum operator * ( float a ) const
	{
		CoefficientSpectrum result = *this;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] *= a;
		}

		return result;
	}

	CoefficientSpectrum& operator *= ( float a ) const
	{
		Assert( !isnan( a ) );

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] *= a;
		}

		return ( *this );
	}

	CoefficientSpectrum operator / ( const CoefficientSpectrum& rhs ) const
	{
		Assert( !rhs.HasNAN() );

		CoefficientSpectrum result = *this;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] /= rhs.c[i];
		}

		return result;
	}

	// Float * CoefficientSpectrum
	friend inline CoefficientSpectrum operator * ( float a , const CoefficientSpectrum& rhs )
	{
		Assert( !isnan( a ) && !rhs.HasNAN() );

		return rhs * a;
	}

	CoefficientSpectrum operator / ( float a ) const
	{
		Assert( !isnan( a ) );

		CoefficientSpectrum result = *this;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] /= a;
		}

		Assert( !result.HasNAN() );

		return result;
	}

	CoefficientSpectrum operator /= ( float a )
	{
		Assert( !isnan( a ) );

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			c[i] /= a;
		}

		return ( *this );
	}

	bool operator == ( const CoefficientSpectrum& rhs ) const
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			if( c[i] != rhs.c[i] )
			{
				return false;
			}
		}

		return true;
	}
	
	bool operator != ( const CoefficientSpectrum& rhs ) const
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			if( c[i] == rhs.c[i] )
			{
				return false;
			}
		}

		return true;
	}
	
	bool IsBlack() const
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			if( c[i] != 0. )
			{
				return false;
			}
		}

		return true;s
	}

	template< int N>
	friend inline CoefficientSpectrum<N> Pow( const CoefficientSpectrum<N>& rhs , float e );

	friend CoefficientSpectrum Exp( const CoefficientSpectrum& rhs )
	{
		CoefficientSpectrum result;

		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			result.c[i] = std::exp( rhs.c[i] );
		}

		Assert( !result.HasNAN() );

		return result;
	}

	bool Write( FILE* t ) const
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			if( fprintf( f , "%f " , c[i] ) < 0 )
			{
				return false;
			}

			return true;
		}
	}

	bool Read( FILE* f )
	{
		for( int i = 0; i < NSpectrumSamples; i++ )
		{
			double v;
			if( fscanf( f , "%lf " , &v ) != 1 )
			{
				return false;
			}

			c[i] = v;
		}

		return true;
	}

	float& operator[] ( int i )
	{
		Assert( i >= 0 && i < NSpectrumSamples );

		return c[i];
	}

	float operator[]( int i ) const
	{
		Assert( i >= 0 && i < NSpectrumSamples );

		return c[i];
	}

	static const int NSamples = NSpectrumSamples;

protected:
	// 一组基函数的值，如RGB的基函数为三个，则只记录rgb三个基函数上的取值
	float c[NSpectrumSamples];
};

class RGBSpectrum : public CoefficientSpectrum<3>
{
	using CoefficientSpectrum<3>::c;

public:
	RGBSpectrum( float v = 0.0f )
		:CoefficientSpectrum<3>( v )
	{

	}

	RGBSpectrum(const CoefficientSpectrum<3>& v) 
		:CoefficientSpectrum<3>( v )
	{

	}

	RGBSpectrum( const RGBSpectrum& s , SpectrumType type = SpectrumType::Reflectance )
	{
		*this = s;
	}

	static RGBSpectrum FromRGB( const float rgb[3] , SpectrumType type = SpectrumType::Reflectance )
	{
		RGBSpectrum s;
		s.c[0] = rgb[0];
		s.c[1] = rgb[1];
		s.c[2] = rgb[2];

		Assert( !s.HasNAN() );

		return s;
	}

	void ToRGB( float* rgb ) const
	{
		rgb[0] = c[0];
		rgb[1] = c[1];
		rgb[2] = c[2];
	}

	void ToXYZ( float xyz[3] ) const
	{
		RGBToXYZ( c , xyz );
	}

	static RGBSpectrum FromXYZ( const float xyz[3] , SpectrumType type = SpectrumType::Reflectance )
	{
		RGBSpectrum r;
		XYZToRGB( xyz , r.c );
		return r;
	}

	float y() const
	{
		const float YWeight[3] = { 0.212671f , 0.715160f , 0.072169f };
		return YWeight[0] * c[0] + YWeight[1] * c[1] + YWeight[2] * c[2];
	}

	static RGBSpectrum FromSampled( const float* lambda , const float* v , int n )
	{
		// 计算三色刺激值xyz，然后将其转换到rgb空间
		
		if( !IsOrdered( lambda , n ) )
		{
			// 波长是未排序的
			std::vector<float> slambda( &lambda[0] , &lambda[n] );
			std::vector<float> sv( &v[0] , &v[n] );

			// 排序光谱采样点
			SortSpectrumSamplePoint( &slambda[0] , &sv[0] , n );

			return FromSampled( &slambda[0] , &sv[0] , n );
		}

		// 此时光谱采样点已经按照波长排序好
		float xyz[3] = { 0 , 0 , 0 };

		float yintergral = 0.0;

		for( int i = 0; i < CIESamplesNum; i++ )
		{
			yintergral += CIE_Y[i];

			float value = InterpolateSpectrumSamples( lambda , v , n , CIE_lambda[i] );
			xyz[0] += value * CIE_X[i];
			xyz[1] += value * CIE_Y[i];
			xyz[2] += value * CIE_Z[i];
		}

		xyz[0] /= yintergral;
		xyz[1] /= yintergral;
		xyz[2] /= yintergral;

		return FromXYZ( xyz );
		
	}
};

typedef RGBSpectrum Spectrum;