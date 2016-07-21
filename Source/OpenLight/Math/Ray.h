#ifndef RAY_H
#define RAY_H

#include "Point3.h"
#include "Vector3.h"

template<typename T>
class Ray
{
public:
	Ray();

	Ray( const Point3<T> &Origin , const Vector3<T> &Direction , T tMin = 1e-3 , T tMax = Infinity , T time = 0.0f , int depth = 0 );

	Point3<T> operator()( T t ) const;

	Ray( const Point3<T>& Origin , const Vector3<T>& Direction , const Ray& parent , T tmin , T tmax = Infinity );

	Ray( const Ray& ray );

public:
	Point3<T> Origin;
	Vector3<T> Direction;
	T time;
	mutable T MinT , MaxT;
	int depth;
};

#include "Ray.inl"
#endif