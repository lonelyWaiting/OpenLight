#pragma once

#ifndef POINT2_H
#define POINT2_H

class Vector2f;

class Point2f
{
public:

	/**
	 \fn	Point2::Point2()

	 \brief	Default constructor.

	 \author	liuteng
	 \date	2016/3/13
	 */

	Point2f();

	/**
	 \fn	Point2::Point2( T _x , T _y )

	 \brief	Constructor.

	 \author
	 \date	2016/3/13

	 \param	_x	The x coordinate.
	 \param	_y	The y coordinate.
	 */

	Point2f( float _x , float _y );
	
	/**
	 \fn	explicit Point2::Point2( const Vector2<U> &p )

	 \brief	Copy Constructor.

	 \author	liuteng
	 \date	2016/3/13

	 \param	p	data source.
	 */

	explicit Point2f( const Vector2f &p );

	/**
	 \fn	Point2::Point2( const Point2f &p )

	 \brief	Copy constructor.

	 \author
	 \date	2016/3/13

	 \param	p	data source.
	 */

	Point2f( const Point2f &p );

	/**
	 \fn	Point2f & Point2::operator= ( const Point2f &p )

	 \brief	Assignment operator.

	 \author
	 \date	2016/3/13

	 \param	p	data source.

	 \return	A shallow copy of this object.
	 */

	Point2f & operator = ( const Point2f &p );

public:
	friend std::ostream & operator << ( std::ostream & os , const Point2f&p );

public:
	Point2f operator + ( const Vector2f &v ) const;

	Point2f operator + ( const Point2f &p ) const;

	Point2f& operator += ( const Vector2f &v );

	Vector2f operator - ( const Point2f & v ) const;

	Point2f operator - ( const Vector2f &v ) const;

	Point2f& operator -= ( const Vector2f &v );

	Point2f operator - () const;

	Point2f operator * ( float f ) const;

	Point2f& operator *= ( float f );

	Point2f operator / ( float f ) const;

	Point2f operator /= ( float f );

	float operator[] ( int index ) const;

	float& operator[] ( int index );

	bool operator == ( const Point2f &p ) const;

	bool operator != ( const Point2f &p ) const;

public:
	friend inline float Distance( const Point2f& p1 , const Point2f &p2 );

	friend inline float DistanceSq( const Point2f &p1 , const Point2f &p2 );

	friend inline Point2f operator* ( float f , const Point2f &p );

	friend Point2f Floor( const Point2f &p );

	friend Point2f Ceil( const Point2f &p );

	friend Point2f Lerp( float t , const Point2f &p0 , const Point2f &p1 );

	friend Point2f Min( const Point2f &p0 , const Point2f &p1 );

	friend Point2f Max( const Point2f &p0 , const Point2f &p1 );
public:
	float x , y;
};

#endif