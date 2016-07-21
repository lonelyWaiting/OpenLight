#ifndef BOUND2_H
#define BOUND2_H

#include <limits>

template<typename T>
class Vector2;

template<typename T>
class Point2;

template<typename T>
class Bound2
{
public:
	Bound2();

	Bound2( const Point2<T>& p );

	Bound2( const Point2<T>& p1 , const Point2<T>& p2 );

	explicit operator Bound2() const;

	Vector2<T> Diagonal() const;

	T Area() const
	{
		Vector2<T> d = pMax - pMin;
		return ( d.x * d.y );
	}

	bool MaximumExtentIsWidth() const;

public:
	const Point2<T> & operator[]( int i ) const;

	Point2<T>& operator[]( int i );

	bool operator == ( const Bound2& rhs ) const;

	bool operator != ( const Bound2& rhs ) const;

	Point2<T> Lerp( const Point2<T>& t ) const;

	Vector2<T> Offset( const Point2<T>& p ) const;

	void BoundingSphere( Point2<T>* Center , T *Radius ) const;

public:
	template<typename D>
	friend bool Inside( const Point2<D>& pt , const Bound2<D>& b );

public:
	Point2<T> pMin , pMax;
};

#include "Bound2.inl"
#endif