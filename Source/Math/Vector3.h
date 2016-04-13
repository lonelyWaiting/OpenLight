#ifndef VECTOR3_H
#define VECTOR3_H

class Point3f;

class Vector3f
{
public:
	Vector3f();

	Vector3f( double value );

	Vector3f( double _x , double _y , double _z );

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

	Vector3f operator * ( double scale ) const;

	Vector3f& operator *= ( double scale );

	double operator * ( const Vector3f &rhs ) const;

	Vector3f operator -() const;

	Vector3f operator / ( double f ) const;

	Vector3f& operator /= ( double f );

	double& operator [] ( int index );

	double operator[] ( int index ) const;

	bool operator == ( const Vector3f& rhs ) const;

	bool operator != ( const Vector3f& rhs ) const;

public:
	double LengthSq() const;

	double Length() const;

public:
	friend Vector3f operator* ( double s , const Vector3f &v );

	friend Vector3f Abs( const Vector3f &v );

	friend double Dot( const Vector3f &v1 , const Vector3f &v2 );

	friend double AbsDot( const Vector3f & v1 , const Vector3f& v2 );

	friend Vector3f Cross( const Vector3f &v1 , const Vector3f &v2 );

	friend Vector3f Normalize( const Vector3f& v );

	friend double MinComponent( const Vector3f &v );

	friend double MaxCompoent( const Vector3f &v );

	friend Vector3f Max( const Vector3f &p1 , const Vector3f &p2 );

	friend Vector3f Min( const Vector3f &p1 , const Vector3f &p2 );

	friend Vector3f Permute( const Vector3f &v , int x , int y , int z );

	friend void CoordinateSystem( Vector3f &v1 , Vector3f *v2 , Vector3f *v3 );

	friend int MaxLengthComponent(const Vector3f& v);

public:
	union
	{
		struct
		{
			double x , y , z;
		};

		struct
		{
			double r , g , b;
		};
	};
};
#endif