#pragma once

#include "Math/Bound3.h"
#include "Spectrum/Spectrum.h"
#include "Core/VSerializableObject.h"
#include "Core/VIntersectable.h"
#include "Core/VCustomRTTI.h"

struct IntersectRecord;
class  Transform;
class  Primitive;
class  Ray;

class Shape : public VSerializableObject , public VIntersectable
{
	DECLARE_DYNAMIC_CREATE_BASE( Shape )

public:
	Shape( Spectrum _emmisive = Spectrum( 0 ) );

	Shape( const Transform* ObjectToWorld );

	virtual ~Shape();

	virtual bool Intersect( Ray& ray , IntersectRecord* record ) const;

	bool IsCombinationShape();

	virtual int GetSubShapeCount() const;

	virtual Shape* GetSubShape( int index ) const;

	virtual void SetPrimitive( Primitive* _pPrimitive );

public:
	Transform*	ObjectToWorld;
	Transform*	WorldToObject;
	Spectrum	Emissive;
	Spectrum	SurfaceColor;
	bool		bCombination;			// 是否由其它shape组成
	Primitive*  pPrimitive;				// 所属图元
};