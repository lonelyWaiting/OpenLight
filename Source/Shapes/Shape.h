#pragma once

#include "Math/Bound3.h"
#include "Spectrum/Spectrum.h"
#include "Core/VObject.h"
#include "Core/VIntersectable.h"

struct IntersectRecord;
class  Transform;
class  Ray;

class Shape : public VObject , public VIntersectable
{
public:
	Shape( Spectrum _emmisive = Spectrum( 0 ) );

	Shape( const Transform* ObjectToWorld );

	virtual ~Shape();

	virtual bool Intersect( Ray& ray , IntersectRecord* record ) const;

	bool IsCombinationShape();

	virtual int GetSubShapeCount() const;

	virtual Shape* GetSubShape( int index ) const;

public:
	Transform*	ObjectToWorld;
	Transform*	WorldToObject;
	Spectrum	emmisive;
	Bound3f		BBoxLocal;
	Bound3f		BBoxWorld;
	bool		bCombination;			// 是否由其它shape组成
};