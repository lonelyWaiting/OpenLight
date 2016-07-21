#ifndef BOUND3_H
#define BOUND3_H

#include "Math/Point3.h"

template<typename T>
class Ray;

template<typename T>
class Vector3;

template<typename T>
class Bound3
{
public:
	Bound3();

	Bound3( const Point3<T>& p );

	Bound3( const Point3<T>& p1 , const Point3<T>& p2 );

	const Point3<T>& operator[]( int i ) const;

	Point3<T>& operator[]( int i );

	Point3<T> Corner( int corner ) const;

	/// \brief 对角线
	Vector3<T> Diagonal() const;

	// 包围盒的表面积
	T SurfaceArea() const;

	Vector3<T> Offset( const Point3<T>& p ) const;

	void BoundingSphere( Point3<T>* center , T* radius ) const;

	void ExpendToInclude( const Point3<T>& p );

	void ExpendToInclude( const Bound3& bbox );

	bool IntersectP( const Ray<T>& ray , T* hitt0 = nullptr , T* hitt1 = nullptr ) const;

	bool IntersectP( const Ray<T>& ray , const Vector3<T>& invDir , const int DirectionIsNegative[3] );

public:
	template<typename D>
	friend bool Inside( const Point3<D>& pt , const Bound3<D>& b );

public:
	Point3<T> pMin , pMax;
};

#include "Bound3.inl"
#endif