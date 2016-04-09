#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"

class Matrix3f
{
public:
	Matrix3f();

	Matrix3f( const Matrix3f& Matrix );

	Matrix3f( float fM00 , float fM01 , float fM02 ,
			  float fM10 , float fM11 , float fM12 ,
			  float fM20 , float fM21 , float fM22 );

public:
	Matrix3f& operator = ( const Matrix3f& Matrix );

	float operator[] ( int iPos ) const;

	float& operator[] ( int iPos );

	bool operator == ( const Matrix3f& Matrix ) const;

	bool operator != ( const Matrix3f& Matrix ) const;

	Matrix3f operator * ( const Matrix3f& Matrix ) const;

	Matrix3f& operator *= ( const Matrix3f& rhs );

	friend Vector3f operator * ( const Vector3f& v , const Matrix3f& matrix );

public:
	void SetRow( int iRow , const Vector3f& Vector );

	Vector3f GetRow( int iRow ) const;

	void SetColumn( int iCol , const Vector3f& Vector );

	Vector3f GetColumn( int iCol );

	void Orthonormalize();

public:
	static Matrix3f RotationX( float fTheta );
	static Matrix3f RotationY( float fTheta );
	static Matrix3f RotationZ( float fTheta );

	static Matrix3f Rotation( float ThetaX , float ThetaY , float ThetaZ );
	static Matrix3f RotateAxis( Vector3f& Axis , float fTheta );

	static Matrix3f Identity();
	static Matrix3f Tranpose( const Matrix3f& matrix );

public:
	union
	{
		struct
		{
			float m00 , m01 , m02;
			float m10 , m11 , m12;
			float m20 , m21 , m22;
		};

		float m[9];
	};
};

#endif