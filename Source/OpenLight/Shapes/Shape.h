#pragma once

#include "Math/Bound3.h"
#include "Spectrum/Spectrum.h"
#include "Core/VSerializableObject.h"
#include "Core/VIntersectable.h"
#include "Core/VCustomRTTI.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"

struct IntersectRecord;
class  Transform;
class  Primitive;
struct  LightSample;

class Shape : public VSerializableObject , public VIntersectable
{
	DECLARE_DYNAMIC_CREATE_BASE( Shape )

public:
	Shape( Spectrum _emmisive = Spectrum( 0 ) );

	Shape( const Transform* ObjectToWorld );

	virtual ~Shape();

	virtual bool Intersect( Rayf& ray , IntersectRecord* record ) const;

	bool HasSubShape();

	virtual int GetChildCount() const;

	virtual Shape* GetChild( int index ) const;

	virtual void SetPrimitive( Primitive* _pPrimitive );
	
	virtual float PDF( const Point3f& p , const Vector3f& wi ) const;

	virtual float Area() const;

	virtual Point3f Sample( const Point3f& p , LightSample& lightSample , Vector3f& normalSample );

	virtual Point3f GetPosition();

	void SetPosition( float* pos );

public:
	Transform*	mObjectToWorld;
	Transform*	mWorldToObject;
	bool		mHasSubShape;				// 是否由其它shape组成
	Primitive*  mPrimitivePtr;				// 所属图元
	Point3f		mWorldPos;
};