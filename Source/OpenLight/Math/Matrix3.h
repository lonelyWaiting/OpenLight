#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"
#include "MathHelper.h"

template<typename T>
class Matrix3
{
public:
	Matrix3();

	Matrix3( const Matrix3& Matrix );

	Matrix3( T fM00 , T fM01 , T fM02 ,
			 T fM10 , T fM11 , T fM12 ,
			 T fM20 , T fM21 , T fM22 );

public:
	Matrix3& operator = ( const Matrix3& Matrix );

	T operator[] ( int iPos ) const;

	T& operator[] ( int iPos );

	bool operator == ( const Matrix3& Matrix ) const;

	bool operator != ( const Matrix3& Matrix ) const;

	Matrix3 operator * ( const Matrix3& Matrix ) const;

	Matrix3& operator *= ( const Matrix3& rhs );

public:
	template<typename D>
	friend Vector3<D> operator * ( const Vector3<D>& v , const Matrix3<D>& matrix );

public:
	void SetRow( int iRow , const Vector3<T>& Vector );

	Vector3<T> GetRow( int iRow ) const;

	void SetColumn( int iCol , const Vector3<T>& Vector );

	Vector3<T> GetColumn( int iCol );

	void Orthonormalize();

public:
	static Matrix3 RotationX( T fTheta );
	static Matrix3 RotationY( T fTheta );
	static Matrix3 RotationZ( T fTheta );

	static Matrix3 Rotation( T ThetaX , T ThetaY , T ThetaZ );
	static Matrix3 RotateAxis( Vector3<T>& Axis , T fTheta );

	static Matrix3 Identity();
	static Matrix3 Tranpose( const Matrix3& matrix );

public:
	union
	{
		struct
		{
			T m00 , m01 , m02;
			T m10 , m11 , m12;
			T m20 , m21 , m22;
		};

		T m[9];
	};
};

#include "Matrix3.inl"
#endif