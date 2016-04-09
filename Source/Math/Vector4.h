#ifndef VECTOR4_H
#define VECTOR4_H

class Vector3f;

class Vector4f
{
public:
	Vector4f();

	Vector4f( float a );

	Vector4f( float x , float y , float z , float w );

	Vector4f( const Vector4f& v );

	Vector4f( const Vector3f& v );

public:
	bool operator == ( const Vector4f& v ) const;

	bool operator != ( const Vector4f& v ) const;

	const Vector4f& operator = ( const Vector4f& v );

	const Vector4f& operator += ( const Vector4f& v );

	Vector4f operator + ( const Vector4f& v ) const;

	const Vector4f& operator -= ( const Vector4f& v );

	Vector4f operator - ( const Vector4f& v ) const;

	Vector4f operator - () const;

	const Vector4f& operator *= ( const Vector4f& v );

	const Vector4f& operator *= ( float a );

	const Vector4f& operator /= ( const Vector4f& v );

	const Vector4f& operator /= ( float a );

	Vector4f operator / ( const Vector4f& v ) const;

	Vector4f operator / ( float a ) const;

	friend std::ostream& operator << ( std::ostream os , const Vector4f& rhs );

public:
	float length() const;

	float lengthSq() const;

	const Vector4f& normalize();

	static unsigned int dimension();

public:
	friend inline float Dot( const Vector4f&v1 , const Vector4f &v2 );

	friend inline float AbsDot( const Vector4f& v1 , const Vector4f& v2 );

	friend Vector4f Abs( const Vector4f& v );

	friend Vector4f Permute( const Vector4f &v , int x , int y , int z , int w );

	friend inline Vector4f Normalize( const Vector4f& v );

public:

	union
	{
		struct
		{
			float x , y , z , w;
		};

		struct
		{
			float r , g , b , a;
		};
	};
};

#endif