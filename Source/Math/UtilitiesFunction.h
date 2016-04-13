#pragma once

#include "math.h"
#include "Normal.h"
#include "Vector3.h"

double AbsDot( const Vector3f& v , const Normal& n )
{
	return fabs( v.x * n.x + v.y * n.y + v.z * n.z );
}

Ray Reflect( const Ray& r , const Normal& n , const Point3f& p , double bias )
{
	Vector3f origDirection = r.Direction;

	Vector3f normal = Vector3f( n.x , n.y , n.z );

	normal = Normalize( normal );

	Vector3f ReflectDir = origDirection - normal * 2 * Dot( origDirection , normal );

	return Ray( p + normal * bias , ReflectDir , r.MinT , r.MaxT , r.time , r.depth + 1 );
}