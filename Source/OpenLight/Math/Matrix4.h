#ifndef MATRIX4_H
#define MATRIX4_H

template<typename T>
class Vector4;

template<typename T>
class Point3;

template<typename T>
class Vector3;

template<typename T>
class Matrix4
{
public:
	Matrix4();

	Matrix4( const T arr[4][4] );

	Matrix4( const T arr[16] );

	Matrix4( T _m00 , T _m01 , T _m02 , T _m03 ,
			 T _m10 , T _m11 , T _m12 , T _m13 ,
			 T _m20 , T _m21 , T _m22 , T _m23 ,
			 T _m30 , T _m31 , T _m32 , T _m33 );

	Matrix4( const Matrix4& matrix );

	Matrix4& operator = ( const Matrix4& rhs );

public:
	bool operator == ( const Matrix4& rhs ) const;

	bool operator != ( const Matrix4& rhs ) const;

	Matrix4 operator * ( const Matrix4& rhs ) const;

public:
	static Matrix4 Mul( const Matrix4& lhs , const Matrix4& rhs );

public:
	static Matrix4 Rotate( T fThetaX , T fThetaY , T fThetaZ );

	static Matrix4 RotateX( T fTheta );

	static Matrix4 RotateY( T fTheta );

	static Matrix4 RotateZ( T fTheta );

	static Matrix4 RotateAxis( const Vector3<T>& axis , T fTheta );

	static Matrix4 Scale( T fScaleX = 1.0f , T fScaleY = 1.0f , T fScaleZ = 1.0f );

	static Matrix4 Translate( T fx = 0.0f , T fy = 0.0f , T fz = 0.0f );

public:
	static Matrix4 LookAtLHMatrix( Point3<T>& eye , Point3<T>& at , Vector3<T>& up );

public:
	template<typename D>
	friend Matrix4<D> Transpose( const Matrix4<D>& rhs );

	template<typename D>
	friend void MakeTranspose( Matrix4<D>& rhs );

	template<typename D>
	friend Matrix4<D> Inverse( const Matrix4<D>& rhs );

	template<typename D>
	friend Vector3<D> operator* ( const Vector3<D>& v , const Matrix4<D>& matrix );

	template<typename D>
	friend Point3<D> operator* ( const Point3<D>& p , const Matrix4<D>& matrix );

	template<typename D>
	friend Vector4<D> operator* ( const Vector4<D>& v , const Matrix4<D>& matrix );

public:
	void SetRow( int i , const Vector3<T>& v );

	void SetColumn( int i , const Vector3<T>& v );

	void SetTranslate( const Vector3<T>& v );

public:
	union
	{
		struct
		{
			T m00 , m01 , m02 , m03;
			T m10 , m11 , m12 , m13;
			T m20 , m21 , m22 , m23;
			T m30 , m31 , m32 , m33;
		};

		T m[16];
	};
};

#include "Matrix4.inl"
#endif