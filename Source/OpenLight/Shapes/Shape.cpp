#include "Utilities/PCH.h"
#include "IO/Log.h"
#include "Math/Normal.h"
#include "Math/Point3.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"
#include "Primitive/IntersectRecord.h"
#include "Light/Light.h"
#include "Shape.h"

Shape::Shape(Spectrum _emmisive /*= Spectrum(0)*/)
{
	ObjectToWorld = new Transform;

	WorldToObject = new Transform;

	bSubShape = false;
}

Shape::Shape( const Transform* _ObjectToWorld )
{
	// Éî¿½±´
	memcpy( ObjectToWorld , _ObjectToWorld , sizeof( Transform ) );

	*WorldToObject = Inverse( *ObjectToWorld );
}

Shape::~Shape()
{
	SAFE_DELETE( ObjectToWorld );
	SAFE_DELETE( WorldToObject );
}

bool Shape::Intersect( Ray& ray , IntersectRecord* record ) const
{
	Assert( "Called Unimplemented Shape::Intersect()" );

	return false;
}

bool Shape::HasSubShape()
{
	return bSubShape;
}

int Shape::GetChildCount() const
{
	return 0;
}

Shape* Shape::GetChild( int index ) const
{
	return nullptr;
}

void Shape::SetPrimitive( Primitive* _pPrimitive )
{
	pPrimitive = _pPrimitive;
}

double Shape::PDF( const Point3f& p , const Vector3f& wi ) const
{
	// First , Interset Test
	Ray r( p , wi );
	IntersectRecord record;
	if( !Intersect( r , &record ) )
	{
		// No Intersect occur
		return 0;
	}

	// Compute PDF
	// \frac{d\omega_i}{dA} = \frac{cos\theta}{r^2}
	// \theta is the angle between the normal of surface hit point and ray direction
	// so d\omega_i = \frac{cos\theta dA}{r^2}
	// because \int pdf d\omega = 1
	// so , \int_0^A pdf \frac{cos\theta dA}{r^2} = 1
	// because we use uniform samling method 
	// so pdf is constant value
	// pdf \int_0^A \frac{cos\theta dA}{r^2} = 1
	// pdf \frac{A cos\theta}{r^2} = 1
	// pdf = \frac{r^2}{A cos\theta}

	double pdf = ( record.HitPoint - p ).LengthSq() / ( Area() * AbsDot( record.normal , -wi ) );

	if( isNanN( pdf ) )
	{
		pdf = 0.0;
	}

	return pdf;
}

double Shape::Area() const
{
	Assert( "the unimplement shape::area() be called" );
	return 0;
}

Point3f Shape::Sample( const Point3f& p , LightSample& lightSample , Normal& normalSample )
{
	// unimplement method
	return Point3f();
}

Point3f Shape::GetPosition()
{
	return Pos;
}

Spectrum Shape::GetSurfaceColor()
{
	return SurfaceColor;
}

void Shape::SetPosition( float* pos )
{
	Pos[0] = ( double )pos[0];
	Pos[1] = ( double )pos[1];
	Pos[2] = ( double )pos[2];

	*ObjectToWorld = Translate( Vector3f( Pos ) );

	*WorldToObject = Inverse( *ObjectToWorld );

	BBoxWorld = ( *ObjectToWorld )( BBoxLocal );
}

void Shape::SetSurfaceColor( float* color )
{
	SurfaceColor = Spectrum::FromRGB( ( double )color[0] , ( double )color[1] , ( double )color[2] );
}