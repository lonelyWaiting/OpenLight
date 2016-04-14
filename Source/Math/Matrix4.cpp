#include "PCH.h"
#include "Point3.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4.h"

inline Matrix4f::Matrix4f()
	: m00( 1 ) , m01( 0 ) , m02( 0 ) , m03( 0 )
	, m10( 0 ) , m11( 1 ) , m12( 0 ) , m13( 0 )
	, m20( 0 ) , m21( 0 ) , m22( 1 ) , m23( 0 )
	, m30( 0 ) , m31( 0 ) , m32( 0 ) , m33( 1 )
{

}

inline Matrix4f::Matrix4f( const double arr[4][4] )
{
	memcpy( m , arr , 16 * sizeof( double ) );
}

inline Matrix4f::Matrix4f( const double arr[16] )
{
	memcpy( m , arr , 16 * sizeof( double ) );
}

inline Matrix4f::Matrix4f( double _m00 , double _m01 , double _m02 , double _m03 ,
				 double _m10 , double _m11 , double _m12 , double _m13 ,
				 double _m20 , double _m21 , double _m22 , double _m23 ,
				 double _m30 , double _m31 , double _m32 , double _m33 )
{
	m00 = _m00;	m01 = _m01;	m02 = _m02;	m03 = _m03;

	m10 = _m10;	m11 = _m11;	m12 = _m12;	m13 = _m13;

	m20 = _m20;	m21 = _m21;	m22 = _m22; m23 = _m23;

	m30 = _m30;	m31 = _m31;	m32 = _m32;	m33 = _m33;
}

inline Matrix4f::Matrix4f( const Matrix4f& matrix )
{
	memcpy( m , matrix.m , 16 * sizeof( double ) );
}

Matrix4f& Matrix4f::operator = ( const Matrix4f& rhs )
{
	memcpy( &m[0] , &( rhs.m[0] ) , 16 * sizeof( double ) );

	return ( *this );
}

bool Matrix4f::operator == ( const Matrix4f& rhs ) const
{
	return ( ( m00 == rhs.m00 ) && ( m01 == rhs.m01 ) && ( m02 == rhs.m02 ) && ( m03 == rhs.m03 ) &&
			 ( m10 == rhs.m10 ) && ( m11 == rhs.m11 ) && ( m12 == rhs.m12 ) && ( m13 == rhs.m13 ) &&
			 ( m20 == rhs.m20 ) && ( m21 == rhs.m21 ) && ( m22 == rhs.m22 ) && ( m23 == rhs.m23 ) &&
			 ( m30 == rhs.m30 ) && ( m31 == rhs.m31 ) && ( m32 == rhs.m32 ) && ( m33 == rhs.m33 ) );
}

bool Matrix4f::operator != ( const Matrix4f& rhs ) const
{
	return ( ( m00 != rhs.m00 ) || ( m01 != rhs.m01 ) || ( m02 != rhs.m02 ) || ( m03 != rhs.m03 ) ||
			 ( m10 != rhs.m10 ) || ( m11 != rhs.m11 ) || ( m12 != rhs.m12 ) || ( m13 != rhs.m13 ) ||
			 ( m20 != rhs.m20 ) || ( m21 != rhs.m21 ) || ( m22 != rhs.m22 ) || ( m23 != rhs.m23 ) ||
			 ( m30 != rhs.m30 ) || ( m31 != rhs.m31 ) || ( m32 != rhs.m32 ) || ( m33 != rhs.m33 ) );
}

Matrix4f Matrix4f::operator * ( const Matrix4f& rhs ) const
{
	return Matrix4f( m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30 ,
					 m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31 ,
					 m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32 ,
					 m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33 ,

					 m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30 ,
					 m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31 ,
					 m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32 ,
					 m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33 ,

					 m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30 ,
					 m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31 ,
					 m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32 ,
					 m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33 ,

					 m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30 ,
					 m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31 ,
					 m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32 ,
					 m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33 );
}

Matrix4f Matrix4f::Mul( const Matrix4f& lhs , const Matrix4f& rhs )
{
	return Matrix4f( lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30 ,
					 lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31 ,
					 lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32 ,
					 lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33 ,

					 lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30 ,
					 lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31 ,
					 lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32 ,
					 lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33 ,

					 lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30 ,
					 lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31 ,
					 lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32 ,
					 lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33 ,

					 lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30 ,
					 lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31 ,
					 lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32 ,
					 lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33 );
}

Matrix4f Inverse( const Matrix4f& matrix )
{
	double fA0 = matrix.m[0] * matrix.m[5] - matrix.m[1] * matrix.m[4];
	double fA1 = matrix.m[0] * matrix.m[6] - matrix.m[2] * matrix.m[4];
	double fA2 = matrix.m[0] * matrix.m[7] - matrix.m[3] * matrix.m[4];
	double fA3 = matrix.m[1] * matrix.m[6] - matrix.m[2] * matrix.m[5];
	double fA4 = matrix.m[1] * matrix.m[7] - matrix.m[3] * matrix.m[5];
	double fA5 = matrix.m[2] * matrix.m[7] - matrix.m[3] * matrix.m[6];
	double fB0 = matrix.m[8] * matrix.m[13] - matrix.m[9] * matrix.m[12];
	double fB1 = matrix.m[8] * matrix.m[14] - matrix.m[10] * matrix.m[12];
	double fB2 = matrix.m[8] * matrix.m[15] - matrix.m[11] * matrix.m[12];
	double fB3 = matrix.m[9] * matrix.m[14] - matrix.m[10] * matrix.m[13];
	double fB4 = matrix.m[9] * matrix.m[15] - matrix.m[11] * matrix.m[13];
	double fB5 = matrix.m[10] * matrix.m[15] - matrix.m[14] * matrix.m[11];

	Matrix4f kInv;
	kInv.m00 = +matrix.m[5] * fB5 - matrix.m[6] * fB4 + matrix.m[7] * fB3;
	kInv.m10 = -matrix.m[4] * fB5 + matrix.m[6] * fB2 - matrix.m[7] * fB1;
	kInv.m20 = +matrix.m[4] * fB4 - matrix.m[5] * fB2 + matrix.m[7] * fB0;
	kInv.m30 = -matrix.m[4] * fB3 + matrix.m[5] * fB1 - matrix.m[6] * fB0;

	kInv.m01 = -matrix.m[1] * fB5 + matrix.m[2] * fB4 - matrix.m[3] * fB3;
	kInv.m11 = +matrix.m[0] * fB5 - matrix.m[2] * fB2 + matrix.m[3] * fB1;
	kInv.m21 = -matrix.m[0] * fB4 + matrix.m[1] * fB2 - matrix.m[3] * fB0;
	kInv.m31 = +matrix.m[0] * fB3 - matrix.m[1] * fB1 + matrix.m[2] * fB0;

	kInv.m02 = +matrix.m[13] * fA5 - matrix.m[14] * fA4 + matrix.m[15] * fA3;
	kInv.m12 = -matrix.m[12] * fA5 + matrix.m[14] * fA2 - matrix.m[15] * fA1;
	kInv.m22 = +matrix.m[12] * fA4 - matrix.m[13] * fA2 + matrix.m[15] * fA0;
	kInv.m32 = -matrix.m[12] * fA3 + matrix.m[13] * fA1 - matrix.m[14] * fA0;

	kInv.m03 = -matrix.m[9] * fA5 + matrix.m[10] * fA4 - matrix.m[11] * fA3;
	kInv.m13 = +matrix.m[8] * fA5 - matrix.m[10] * fA2 + matrix.m[11] * fA1;
	kInv.m23 = -matrix.m[8] * fA4 + matrix.m[9] * fA2 - matrix.m[11] * fA0;
	kInv.m33 = +matrix.m[8] * fA3 - matrix.m[9] * fA1 + matrix.m[10] * fA0;

	double fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;

	double fInvDet = ( ( double )1.0f ) / fDet;
	for( int i = 0; i < 16; i++ )
	{
		kInv.m[i] *= fInvDet;
	}

	return kInv;
}

Matrix4f Transpose( const Matrix4f& rhs )
{
	Matrix4f Trans;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			Trans.m[iRow * 4 + iCol] = rhs.m[iCol * 4 + iRow];
		}
	}

	return Trans;
}

void MakeTranspose( Matrix4f& rhs )
{
	Matrix4f Trans;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			Trans.m[iRow * 4 + iCol] = rhs.m[iCol * 4 + iRow];
		}
	}

	memcpy( &( rhs.m[0] ) , &( Trans.m[0] ) , 16 * sizeof( double ) );
}

Matrix4f Matrix4f::Rotate( double fThetaX , double fThetaY , double fThetaZ )
{
	double sinThetaX = sinf( ToRadians( fThetaX ) );
	double cosThetaX = cosf( ToRadians( fThetaX ) );
	double sinThetaY = sinf( ToRadians( fThetaY ) );
	double cosThetaY = cosf( ToRadians( fThetaY ) );
	double sinThetaZ = sinf( ToRadians( fThetaZ ) );
	double cosThetaZ = cosf( ToRadians( fThetaZ ) );
	
	double cosYMulCosZ = cosThetaY * cosThetaZ;
	double sinYMulCosZ = sinThetaY * cosThetaZ;
	double sinXMulCosZ = sinThetaX * cosThetaZ;
	double sinXMulSinZ = sinThetaX * sinThetaZ;

	return Matrix4f(	cosYMulCosZ + sinXMulSinZ * sinThetaY				, cosThetaX * sinThetaZ		, -sinYMulCosZ + sinXMulSinZ * cosThetaY			, 0.0f ,
						-cosThetaY * sinThetaZ + sinThetaX * sinYMulCosZ	, cosThetaX *cosThetaZ		, sinThetaY * sinThetaZ + cosYMulCosZ * sinThetaX	, 0.0f ,
						cosThetaX * sinThetaY								, -sinThetaX				, cosThetaX * cosThetaY								, 0.0f ,
						0.0f												, 0.0f						, 0.0f												, 1.0f );
}

Matrix4f Matrix4f::RotateAxis( const Vector3f& axis , double fTheta )
{
	Vector3f a = Normalize( axis );
	double x = a.x;
	double y = a.y;
	double z = a.z;

	double cosTheta = std::cos( ToRadians( fTheta ) );
	double OneSubCos = 1.0f - cosTheta;
	double sinTheta = std::sin( ToRadians( fTheta ) );

	return Matrix4f(	x * x * OneSubCos + cosTheta		, x * y * OneSubCos + z * sinTheta	, x * z * OneSubCos - y * sinTheta	, 0.0f ,
						x * y * OneSubCos - z * sinTheta	, y * y * OneSubCos + cosTheta		, y * z * OneSubCos + x * sinTheta	, 0.0f ,
						x * z * OneSubCos + y * sinTheta	, y * z * OneSubCos - x * sinTheta	, z * z * OneSubCos + cosTheta		, 0.0f ,
						0.0f								, 0.0f								, 0.0f								, 1.0f );
}

Matrix4f Matrix4f::RotateX( double fTheta )
{
	double fSin = sinf( ToRadians( fTheta ) );
	double fCos = cosf( ToRadians( fTheta ) );

	return Matrix4f( 1.0f , 0.0f , 0.0f , 0.0f ,
					 0.0f , fCos , fSin , 0.0f ,
					 0.0f , -fSin , fCos , 0.0f ,
					 0.0f , 0.0f , 0.0f , 1.0f );
}

Matrix4f Matrix4f::RotateY( double fTheta )
{
	double fSin = sinf( ToRadians( fTheta ) );
	double fCos = cosf( ToRadians( fTheta ) );

	return Matrix4f( fCos , 0.0f , -fSin , 0.0f ,
					 0.0f , 1.0f , 0.0f , 0.0f ,
					 fSin , 0.0f , fCos , 0.0f ,
					 0.0f , 0.0f , 0.0f , 1.0f );
}

Matrix4f Matrix4f::RotateZ( double fTheta )
{
	double fSin = sinf( ToRadians(fTheta) );
	double fCos = cosf( ToRadians( fTheta ) );

	return Matrix4f( fCos , fSin , 0.0f , 0.0f ,
					 -fSin , fCos , 0.0f , 0.0f ,
					 0.0f , 0.0f , 1.0f , 0.0f ,
					 0.0f , 0.0f , 0.0f , 1.0f );
}

Matrix4f Matrix4f::Scale( double fScaleX , double fScaleY , double fScaleZ )
{
	return Matrix4f( fScaleX , 0.0f , 0.0f , 0.0f ,
					 0.0f , fScaleY , 0.0f , 0.0f ,
					 0.0f , 0.0f , fScaleZ , 0.0f ,
					 0.0f , 0.0f , 0.0f , 1.0f );
}

Matrix4f Matrix4f::Translate( double fx , double fy , double fz )
{
	return Matrix4f( 1.0f , 0.0f , 0.0f , 0.0f ,
					 0.0f , 1.0f , 0.0f , 0.0f ,
					 0.0f , 0.0f , 1.0f , 0.0f ,
					 fx , fy , fz , 1.0f );
}

inline Vector4f operator * ( const Vector4f& v , const Matrix4f& matrix )
{
	return Vector4f( v.x * matrix.m00 + v.y * matrix.m10 + v.z * matrix.m20 + v.w * matrix.m30 ,
					   v.x * matrix.m01 + v.y * matrix.m11 + v.z * matrix.m21 + v.w * matrix.m31 ,
					   v.x * matrix.m02 + v.y * matrix.m12 + v.z * matrix.m22 + v.w * matrix.m32 ,
					   v.x * matrix.m03 + v.y * matrix.m13 + v.z * matrix.m23 + v.w * matrix.m33 );
}

Point3f operator * ( const Point3f& p , const Matrix4f& matrix )
{
	double w = p.x * matrix.m03 + p.y * matrix.m13 + p.z * matrix.m23 + matrix.m33;

	if( w == 1 )
	{
		return Point3f( p.x * matrix.m00 + p.y * matrix.m10 + p.z * matrix.m20 + matrix.m30 ,
						  p.x * matrix.m01 + p.y * matrix.m11 + p.z * matrix.m21 + matrix.m31 ,
						  p.x * matrix.m02 + p.y * matrix.m12 + p.z * matrix.m22 + matrix.m32 );
	}

	assert( w != 0.0f );

	double inv = 1.0f / w;

	return Point3f( ( p.x * matrix.m00 + p.y * matrix.m10 + p.z * matrix.m20 + matrix.m30 ) * inv ,
					  ( p.x * matrix.m01 + p.y * matrix.m11 + p.z * matrix.m21 + matrix.m31 ) * inv ,
					  ( p.x * matrix.m02 + p.y * matrix.m12 + p.z * matrix.m22 + matrix.m32 ) * inv );
}

Vector3f operator* ( const Vector3f& v , const Matrix4f& matrix )
{
	double x = v.x , y = v.y , z = v.z;

	// 向量没有位移
	return Vector3f( ( x * matrix.m00 + y * matrix.m10 + z * matrix.m20 ) ,
					 ( x * matrix.m01 + y * matrix.m11 + z * matrix.m21 ) ,
					 ( x * matrix.m02 + y * matrix.m12 + z * matrix.m22 ) );
}