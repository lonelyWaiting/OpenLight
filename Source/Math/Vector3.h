#ifndef VECTOR3_H
#define VECTOR3_H

class Point3f;

class Vector3f
{
public:
	Vector3f();

	Vector3f( float value );

	Vector3f( float _x , float _y , float _z );

	Vector3f( const Vector3f& rhs );

	explicit Vector3f( const Point3f& p );

public:
	friend std::ostream & operator << ( std::ostream & os , const Vector3f &v );

public:
	Vector3f& operator = ( const Vector3f& rhs );

	Vector3f operator + ( const Vector3f& rhs ) const;

	Vector3f& operator += ( const Vector3f& rhs );

	Vector3f operator - ( const Vector3f& rhs ) const;

	Vector3f& operator -= ( const Vector3f& rhs );

	Vector3f operator * ( float scale ) const;

	Vector3f& operator *= ( float scale );

	float operator * ( const Vector3f &rhs ) const;

	Vector3f operator -() const;

	Vector3f operator / ( float f ) const;

	Vector3f& operator /= ( float f );

	float& operator [] ( int index );

	float operator[] ( int index ) const;

	bool operator == ( const Vector3f& rhs ) const;

	bool operator != ( const Vector3f& rhs ) const;

public:
	float LengthSq() const;

	float Length() const;

public:
	friend Vector3f operator* ( float s , const Vector3f &v );

	friend Vector3f Abs( const Vector3f &v );

	friend float Dot( const Vector3f &v1 , const Vector3f &v2 );

	friend float AbsDot( const Vector3f & v1 , const Vector3f& v2 );

	friend Vector3f Cross( const Vector3f &v1 , const Vector3f &v2 );

	friend Vector3f Normalize( const Vector3f& v );

	friend float MinComponent( const Vector3f &v );

	friend float MaxCompoent( const Vector3f &v );

	friend Vector3f Max( const Vector3f &p1 , const Vector3f &p2 );

	friend Vector3f Min( const Vector3f &p1 , const Vector3f &p2 );

	friend Vector3f Permute( const Vector3f &v , int x , int y , int z );

	friend void CoordinateSystem( Vector3f &v1 , Vector3f *v2 , Vector3f *v3 );
public:
	union
	{
		struct
		{
			float x , y , z;
		};

		struct
		{
			float r , g , b;
		};
	};
};
#endif