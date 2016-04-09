#pragma once

class Vector2f;
class Point2f;

class Bound2f
{
public:
	Bound2f();

	Bound2f( const Point2f& p );

	Bound2f( const Point2f& p1 , const Point2f& p2 );

	explicit operator Bound2f() const;

	// 对角线
	Vector2f Diagonal() const;


	// 面积
	float Area() const
	{
		Vector2f d = pMax - pMin;
		return ( d.x * d.y );
	}

	// 检测最大的边
	bool MaximumExtentIsWidth() const;

public:
	inline const Point2f & operator[]( int i ) const;

	inline Point2f& operator[]( int i );

	bool operator == ( const Bound2f& rhs ) const;

	bool operator != ( const Bound2f& rhs ) const;

	Point2f Lerp( const Point2f& t ) const;

	Vector2f Offset( const Point2f& p ) const;

	void BoundingSphere( Point2f* Center , float *Radius ) const;
	
public:
	friend bool Inside( const Point2f& pt , const Bound2f& b );

public:
	Point2f pMin , pMax;
};