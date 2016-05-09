#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"

class Matrix3f
{
public:
	Matrix3f();

	Matrix3f( const Matrix3f& Matrix );

	Matrix3f( double fM00 , double fM01 , double fM02 ,
			  double fM10 , double fM11 , double fM12 ,
			  double fM20 , double fM21 , double fM22 );

public:
	Matrix3f& operator = ( const Matrix3f& Matrix );

	double operator[] ( int iPos ) const;

	double& operator[] ( int iPos );

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
	static Matrix3f RotationX( double fTheta );
	static Matrix3f RotationY( double fTheta );
	static Matrix3f RotationZ( double fTheta );

	static Matrix3f Rotation( double ThetaX , double ThetaY , double ThetaZ );
	static Matrix3f RotateAxis( Vector3f& Axis , double fTheta );

	static Matrix3f Identity();
	static Matrix3f Tranpose( const Matrix3f& matrix );

public:
	union
	{
		struct
		{
			double m00 , m01 , m02;
			double m10 , m11 , m12;
			double m20 , m21 , m22;
		};

		double m[9];
	};
};

#endif