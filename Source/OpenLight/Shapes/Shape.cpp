#include "Utilities/PCH.h"
#include "IO/Log.h"
#include "Math/Point3.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"
#include "Primitive/IntersectRecord.h"
#include "Light/Light.h"
#include "Shape.h"

Shape::Shape(Spectrum _emmisive /*= Spectrum(0)*/)
{
	mObjectToWorld = new Transform;

	mWorldToObject = new Transform;

	mHasSubShape = false;
}

Shape::Shape( const Transform* _ObjectToWorld )
{
	// Éî¿½±´
	memcpy( mObjectToWorld , _ObjectToWorld , sizeof( Transform ) );

	*mWorldToObject = Inverse( *mObjectToWorld );
}

Shape::~Shape()
{
	SAFE_DELETE( mObjectToWorld );
	SAFE_DELETE( mWorldToObject );
}

bool Shape::Intersect( Rayf& ray , IntersectRecord* record ) const
{
	Assert( "Called Unimplemented Shape::Intersect()" );

	return false;
}

bool Shape::HasSubShape()
{
	return mHasSubShape;
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
	mPrimitivePtr = _pPrimitive;
}

float Shape::PDF( const Point3f& p , const Vector3f& wi ) const
{
	// First , Interset Test
	Rayf r( p , wi );
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

	float pdf = ( record.HitPoint - p ).LengthSq() / ( Area() * AbsDot( record.normal , -wi ) );

	if( isNanN( pdf ) )
	{
		pdf = 0.0;
	}

	return pdf;
}

float Shape::Area() const
{
	Assert( "the unimplement shape::area() be called" );
	return 0;
}

Point3f Shape::Sample( const Point3f& p , LightSample& lightSample , Vector3f& normalSample )
{
	// unimplement method
	return Point3f();
}

Point3f Shape::GetPosition()
{
	return mWorldPos;
}

void Shape::SetPosition( float* pos )
{
	mWorldPos[0] = ( float )pos[0];
	mWorldPos[1] = ( float )pos[1];
	mWorldPos[2] = ( float )pos[2];

	*mObjectToWorld = Translate( Vector3f( mWorldPos ) );

	*mWorldToObject = Inverse( *mObjectToWorld );

	BBoxWorld = ( *mObjectToWorld )( BBoxLocal );
}