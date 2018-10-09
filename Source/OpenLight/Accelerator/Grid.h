#pragma once

#include "Utilities/PCH.h"
#include "Accelerator.h"
#include <Math/Ray.h>
#include <vector>

class Shape;

class Grid : public Accelerator
{
public:
	virtual void Setup( const Scene* scene );

	virtual bool Intersect( Rayf& ray , const Scene* scene , IntersectRecord* record );

	virtual void Reset();

private:
	std::vector<Shape*> cells;

	int nx , ny , nz;			// x , y , z�����դ����Ŀ

	const Scene* pScene;
};