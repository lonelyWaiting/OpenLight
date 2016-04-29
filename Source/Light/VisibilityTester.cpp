#include "PCH.h"
#include "Core/Scene.h"
#include "Accelerator/Accelerator.h"
#include "VisibilityTester.h"

void VisibilityTester::SetSegment( const Point3f& p1 , double eps1 , const Point3f& p2 , double eps2 )
{
	double dist = ( p2 - p1 ).Length();

	r = Ray( p1 , ( p2 - p1 ) / dist , eps1 , dist + ( 1 - eps2 ) );
}

bool VisibilityTester::Unoccluded( const Scene* scene , Accelerator* pAccelerator ) const
{
	IntersectRecord record;
	if( !pAccelerator->Intersect( r , scene , &record ) )
	{
		// 未发生碰撞，即未被遮挡，返回true
		return true;
	}

	return false;
}