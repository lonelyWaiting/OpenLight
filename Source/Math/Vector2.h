#ifndef VECTOR2_H
#define VECTOR2_H

class Point2f;

class Point3f;

class Vector2f
{
public:
	Vector2f();

	Vector2f( float _x , float _y );

	explicit Vector2f( const Point2f &p );

	explicit Vector2f( const Point3f &p );

	Vector2f( const Vector2f &rhs );

	Vector2f& operator = ( const Vector2f &rhs );

	friend std::ostream& operator << ( std::ostream &os , const Vector2f &rhs );

	Vector2f operator + ( const Vector2f& v ) const;

	Vector2f& operator += ( const Vector2f& v );

	Vector2f operator - ( const Vector2f &v ) const;

	Vector2f& operator -= ( const Vector2f &v );

	bool operator == ( const Vector2f &v ) const;

	bool operator != ( const Vector2f & v ) const;

	Vector2f operator* ( float f ) const;

	Vector2f& operator *= ( float f );

	Vector2f operator / ( float f ) const;

	Vector2f& operator /= ( float f );

	Vector2f operator - () const;

	float& operator [] ( int index );

	float LengthSq() const;

	float Length() const;

public:
	friend inline Vector2f operator* ( float f , const Vector2f &v );

	friend inline float Dot( const Vector2f &v1 , const Vector2f &v2 );

	friend inline float AbsDot( const Vector2f &v1 , const Vector2f &v2 );

	friend inline Vector2f Normalize( const Vector2f &v );

	friend Vector2f Abs( const Vector2f &v );

public:
	float x , y;
};
#endif