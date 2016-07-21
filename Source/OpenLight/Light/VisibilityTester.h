#pragma once

#include "Math/Ray.h"

class Accelerator;
class Scene;

class VisibilityTester
{
public:
	// 设置光线线段
	void SetSegment( const Point3f& p1 , float eps1 , const Point3f& p2 , float eps2 );

	bool Unoccluded( const Scene* scene , Accelerator* pAccelerator ) const;

public:
	mutable Rayf r;
};