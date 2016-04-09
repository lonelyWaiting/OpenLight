#ifndef MATRIX4_H
#define MATRIX4_H

class Vector4f;

class Point3f;

class Vector3f;

class Matrix4f
{
public:
	inline Matrix4f();

	inline Matrix4f( const float arr[4][4] );

	inline Matrix4f( const float arr[16] );

	inline Matrix4f( float _m00 , float _m01 , float _m02 , float _m03 ,
					 float _m10 , float _m11 , float _m12 , float _m13 ,
					 float _m20 , float _m21 , float _m22 , float _m23 ,
					 float _m30 , float _m31 , float _m32 , float _m33 );

	inline Matrix4f( const Matrix4f& matrix );

public:
	bool operator == ( const Matrix4f& rhs ) const;

	bool operator != ( const Matrix4f& rhs ) const;

	Matrix4f operator * ( const Matrix4f& rhs ) const;

public:
	friend Matrix4f Transpose( const Matrix4f& rhs );

	friend void MakeTranspose( Matrix4f& rhs );

	friend Matrix4f Inverse( const Matrix4f& rhs );

	friend Vector3f operator* ( const Vector3f& v , const Matrix4f& matrix );

	static Matrix4f Mul( const Matrix4f& lhs , const Matrix4f& rhs );

public:
	static Matrix4f Rotate( float fThetaX , float fThetaY , float fThetaZ );

	static Matrix4f RotateX( float fTheta );

	static Matrix4f RotateY( float fTheta );

	static Matrix4f RotateZ( float fTheta );

	static Matrix4f RotateAxis( const Vector3f& axis , float fTheta );

	static Matrix4f Scale( float fScaleX = 1.0f , float fScaleY = 1.0f , float fScaleZ = 1.0f );

	static Matrix4f Translate( float fx = 0.0f , float fy = 0.0f , float fz = 0.0f );

public:
	friend Point3f operator* ( const Point3f& p , const Matrix4f& matrix );

	friend Vector4f operator* ( const Vector4f& v , const Matrix4f& matrix );
public:
	union
	{
		struct
		{
			float m00 , m01 , m02 , m03;
			float m10 , m11 , m12 , m13;
			float m20 , m21 , m22 , m23;
			float m30 , m31 , m32 , m33;
		};

		float m[16];
	};
};
#endif