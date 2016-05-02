#include "PCH.h"
#include "Math/Normal.h"
#include "Primitive/Primitive.h"
#include "AreaLighth.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( AreaLight , Light )


AreaLight::AreaLight()
{
	m_pPrimitive = nullptr;
}

AreaLight::AreaLight( const Transform& LightToWorld , const Spectrum& Le )
	:Light( LightToWorld )
{

}

Spectrum AreaLight::Sample_L( const Point3f& p , Vector3f* wi , LightSample& _lightSample , double* pdf , VisibilityTester* pVisibility ) const
{
	// 采样光源对应的图元
	int index = ( int )( _lightSample.value[0] * ( m_pPrimitive->GetShapeCount() - 1 ) );

	Normal LightSourceSampleNormal;
	Point3f LightSourceSamplePoint = m_pPrimitive->GetShape( index )->Sample( p , _lightSample , LightSourceSampleNormal );

	*wi = Normalize( LightSourceSamplePoint - p );

	// Compute Primitive PDF value
	*pdf = m_pPrimitive->PDF( p , *wi );

	pVisibility->SetSegment( p , 1e-3f , LightSourceSamplePoint , 1e-3f );

	// Compute Li
	Spectrum Li = Le( LightSourceSamplePoint , LightSourceSampleNormal , -*wi );

	return Li;
}

// \Phi = \int_A\int_\Omega L(x \rightarrow \Theta )cos\theta d_{\omega_\Theta}dA
// 对于漫反射而言，L(x \rightarrow \Theta) = L
// \Phi = \int_A\int_\Omega Lcos\theta d_{\omega_\Theta}dA
// \Phi = L\int_AdA\int_\Omega cos\theta d_{\omega_\Theta}
// \Phi = LA\pi
Spectrum AreaLight::Power( const Scene* scene ) const
{
	return Lemission * m_pPrimitive->GetArea() * PI;
}

Spectrum AreaLight::Sample_L( const Scene* scene , LightSample& _lightSample , Ray* ray , Normal* NormalShading , double* pdf ) const
{
	return Spectrum( 0.0 );
}

void AreaLight::Deserialization( XMLElement* LightRootElement )
{
	double r , g , b;
	LightRootElement->FirstChildElement( "Le" )->FirstChildElement( "r" )->QueryDoubleText( &r );
	LightRootElement->FirstChildElement( "Le" )->FirstChildElement( "g" )->QueryDoubleText( &g );
	LightRootElement->FirstChildElement( "Le" )->FirstChildElement( "b" )->QueryDoubleText( &b );

	Lemission = Spectrum::FromRGB( r , g , b );
}

Spectrum AreaLight::Le( const Point3f& p , const Normal& n , const Vector3f& wo ) const
{
	return Dot( n , wo ) > 0.0 ? Lemission : 0.0;
}

double AreaLight::PDF( const Point3f& p , const Vector3f& wi ) const
{
	return m_pPrimitive->PDF( p , wi );
}

void AreaLight::SetPrimitive( Primitive* pPrimitive )
{
	m_pPrimitive = pPrimitive;
}