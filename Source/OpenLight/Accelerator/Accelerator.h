#pragma once

class Scene;

// base class
class Accelerator
{
public:
	virtual void Setup( const Scene* scene ) = 0;
	virtual bool Intersect( Rayf& ray , const Scene* scene , IntersectRecord* record ) = 0;

	virtual void Reset() = 0;
};