#ifndef MATRIX4_H
#define MATRIX4_H

class Vector4f;

class Point3f;

class Vector3f;

class Matrix4f
{
public:
	inline Matrix4f();

	inline Matrix4f( const double arr[4][4] );

	inline Matrix4f( const double arr[16] );

	inline Matrix4f( double _m00 , double _m01 , double _m02 , double _m03 ,
					 double _m10 , double _m11 , double _m12 , double _m13 ,
					 double _m20 , double _m21 , double _m22 , double _m23 ,
					 double _m30 , double _m31 , double _m32 , double _m33 );

	inline Matrix4f( const Matrix4f& matrix );

	Matrix4f& operator = ( const Matrix4f& rhs );

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
	static Matrix4f Rotate( double fThetaX , double fThetaY , double fThetaZ );

	static Matrix4f RotateX( double fTheta );

	static Matrix4f RotateY( double fTheta );

	static Matrix4f RotateZ( double fTheta );

	static Matrix4f RotateAxis( const Vector3f& axis , double fTheta );

	static Matrix4f Scale( double fScaleX = 1.0f , double fScaleY = 1.0f , double fScaleZ = 1.0f );

	static Matrix4f Translate( double fx = 0.0f , double fy = 0.0f , double fz = 0.0f );

public:
	friend Point3f operator* ( const Point3f& p , const Matrix4f& matrix );

	friend Vector4f operator* ( const Vector4f& v , const Matrix4f& matrix );
public:
	union
	{
		struct
		{
			double m00 , m01 , m02 , m03;
			double m10 , m11 , m12 , m13;
			double m20 , m21 , m22 , m23;
			double m30 , m31 , m32 , m33;
		};

		double m[16];
	};
};
#endif