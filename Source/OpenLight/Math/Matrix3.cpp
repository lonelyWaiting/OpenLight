#include "Utilities/PCH.h"
#include "Point3.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix3.h"

Matrix3f::Matrix3f()
	: m00( 1 ) , m01( 0 ) , m02( 0 )
	, m10( 0 ) , m11( 1 ) , m12( 0 )
	, m20( 0 ) , m21( 0 ) , m22( 1 )
{

}

Matrix3f::Matrix3f( const Matrix3f& Matrix )
	: m00( Matrix.m00 ) , m01( Matrix.m01 ) , m02( Matrix.m02 )
	, m10( Matrix.m10 ) , m11( Matrix.m11 ) , m12( Matrix.m12 )
	, m20( Matrix.m20 ) , m21( Matrix.m21 ) , m22( Matrix.m22 )
{

}


Matrix3f::Matrix3f( double fM00 , double fM01 , double fM02 ,
					double fM10 , double fM11 , double fM12 ,
					double fM20 , double fM21 , double fM22 )
	: m00( fM00 ) , m01( fM01 ) , m02( fM02 )
	, m10( fM10 ) , m11( fM11 ) , m12( fM12 )
	, m20( fM20 ) , m21( fM21 ) , m22( fM22 )
{

}

Matrix3f& Matrix3f::operator = ( const Matrix3f& Matrix )
{
	memcpy( this->m , Matrix.m , 9 * sizeof( double ) );

	return *this;
}

double Matrix3f::operator[] ( int iPos ) const
{
	return m[iPos];
}

double& Matrix3f::operator[] ( int iPos )
{
	return m[iPos];
}

bool Matrix3f::operator == ( const Matrix3f& Matrix ) const
{
	return ( ( m00 == Matrix.m00 ) && ( m01 == Matrix.m01 ) && ( m02 == Matrix.m02 ) &&
			 ( m10 == Matrix.m10 ) && ( m11 == Matrix.m11 ) && ( m12 == Matrix.m12 ) &&
			 ( m20 == Matrix.m21 ) && ( m21 == Matrix.m21 ) && ( m22 == Matrix.m22 ) );
}

bool Matrix3f::operator != ( const Matrix3f& Matrix ) const
{
	return ( ( m00 != Matrix.m00 ) || ( m01 != Matrix.m01 ) || ( m02 != Matrix.m02 ) ||
			 ( m10 != Matrix.m10 ) || ( m11 != Matrix.m11 ) || ( m12 != Matrix.m12 ) ||
			 ( m20 != Matrix.m21 ) || ( m21 != Matrix.m21 ) || ( m22 != Matrix.m22 ) );
}

Matrix3f Matrix3f::operator * ( const Matrix3f& Matrix ) const
{
	return Matrix3f( m00 * Matrix[0] + m01 * Matrix[3] + m02 * Matrix[6] , m00 * Matrix[1] + m01 * Matrix[4] + m02 * Matrix[7] , m00 * Matrix[2] + m01 * Matrix[5] + m02 * Matrix[8] ,
					 m10 * Matrix[0] + m11 * Matrix[3] + m12 * Matrix[6] , m10 * Matrix[1] + m11 * Matrix[4] + m12 * Matrix[7] , m10 * Matrix[2] + m11 * Matrix[5] + m12 * Matrix[8] ,
					 m20 * Matrix[0] + m21 * Matrix[3] + m22 * Matrix[6] , m20 * Matrix[1] + m21 * Matrix[4] + m22 * Matrix[7] , m20 * Matrix[2] + m21 * Matrix[5] + m22 * Matrix[8] );
}

Matrix3f& Matrix3f::operator *= ( const Matrix3f& rhs )
{
	Matrix3f temp = *this;

	m00 = temp.m00 * rhs.m00 + temp.m01 * rhs.m10 + temp.m02 * rhs.m20;
	m01 = temp.m00 * rhs.m01 + temp.m01 * rhs.m11 + temp.m02 * rhs.m21;
	m02 = temp.m00 * rhs.m02 + temp.m01 * rhs.m12 + temp.m02 * rhs.m22;

	m10 = temp.m10 * rhs.m00 + temp.m11 * rhs.m10 + temp.m12 * rhs.m20;
	m11 = temp.m10 * rhs.m01 + temp.m11 * rhs.m11 + temp.m12 * rhs.m21;
	m12 = temp.m10 * rhs.m02 + temp.m11 * rhs.m12 + temp.m12 * rhs.m22;

	m20 = temp.m20 * rhs.m00 + temp.m21 * rhs.m10 + temp.m22 * rhs.m20;
	m21 = temp.m20 * rhs.m01 + temp.m21 * rhs.m11 + temp.m22 * rhs.m21;
	m22 = temp.m20 * rhs.m02 + temp.m21 * rhs.m12 + temp.m22 * rhs.m22;

	return *this;
}

void Matrix3f::Orthonormalize()
{
	// 使用施密特正交化方法
	double fInvLength = static_cast< double >( 1.0f / sqrt( m[0] * m[0] + m[3] * m[3] + m[6] * m[6] ) );

	m[0] *= fInvLength;
	m[3] *= fInvLength;
	m[6] *= fInvLength;

	double fDot0 = m[0] * m[1] + m[3] * m[4] + m[6] * m[7];

	m[1] -= fDot0 * m[0];
	m[4] -= fDot0 * m[3];
	m[7] -= fDot0 * m[6];

	fInvLength = static_cast< double >( 1.0f / sqrt( m[1] * m[1] + m[4] * m[4] + m[7] * m[7] ) );

	m[1] *= fInvLength;
	m[4] *= fInvLength;
	m[7] *= fInvLength;

	double fDot1 = m[0] * m[2] + m[3] * m[5] + m[6] * m[8];
	double fDot2 = m[1] * m[2] + m[4] * m[5] + m[7] * m[8];

	m[2] -= ( fDot1 * m[0] + fDot2 * m[1] );
	m[5] -= ( fDot1 * m[3] + fDot2 * m[4] );
	m[8] -= ( fDot1 * m[6] + fDot2 * m[7] );

	fInvLength = static_cast< double >( 1.0f / sqrt( m[2] * m[2] + m[5] * m[5] + m[8] * m[8] ) );

	m[2] *= fInvLength;
	m[5] *= fInvLength;
	m[8] *= fInvLength;
}


Matrix3f Matrix3f::RotationX( double fTheta )
{
	double fSin = sin( ToRadians( fTheta ) );
	double fCos = cos( ToRadians( fTheta ) );

	return Matrix3f( 1.0f , 0.0f , 0.0f ,
					 0.0f , fCos , fSin ,
					 0.0f , -fSin , fCos );
}

Matrix3f Matrix3f::RotationY( double fTheta )
{
	double fSin = sin( ToRadians( fTheta ) );
	double fCos = cos( ToRadians( fTheta ) );

	return Matrix3f( fCos , 0.0f , -fSin ,
					 0.0f , 1.0f , 0.0f ,
					 fSin , 0.0f , fCos );
}

Matrix3f Matrix3f::RotationZ( double fTheta )
{
	double fSin = sin( ToRadians( fTheta ) );
	double fCos = cos( ToRadians( fTheta ) );

	return Matrix3f( fCos , fSin , 0.0f ,
					 -fSin , fCos , 0.0f ,
					 0.0f , 0.0f , 1.0f );
}

Matrix3f Matrix3f::Rotation( double ThetaX , double ThetaY , double ThetaZ )
{
	double fSinX = sin( ToRadians( ThetaX ) );
	double fCosX = cos( ToRadians( ThetaX ) );
	double fSinY = sin( ToRadians( ThetaY ) );
	double fCosY = cos( ToRadians( ThetaY ) );
	double fSinZ = sin( ToRadians( ThetaZ ) );
	double fCosZ = cos( ToRadians( ThetaZ ) );

	double fCosYfCosZ = fCosY * fCosZ;
	double fSinYfCosZ = fSinY * fCosZ;
	double fSinXfCosZ = fSinX * fCosZ;
	double fSinXfSinZ = fSinX * fSinZ;

	return Matrix3f( fCosYfCosZ + fSinXfSinZ * fSinY , fCosX * fSinZ , -fSinYfCosZ + fSinXfSinZ * fCosY ,
					 -fCosY * fSinZ + fSinX * fSinYfCosZ , fCosX *fCosZ , fSinY * fSinZ + fCosYfCosZ * fSinX ,
					 fCosX * fSinY , -fSinX , fCosX * fCosY );
}

Matrix3f Matrix3f::RotateAxis( Vector3f & Axis , double fTheta )
{
	double x = Axis.x;
	double y = Axis.y;
	double z = Axis.z;

	double Cos = cos( ToRadians( fTheta ) );
	double OneSubCos = 1 - Cos;
	double Sin = sin( ToRadians( fTheta ) );

	return Matrix3f( x * x * OneSubCos + Cos , x * y * OneSubCos + z * Sin , x * z * OneSubCos - y * Sin ,
					 x * y * OneSubCos - z * Sin , y * y * OneSubCos + Cos , y * z * OneSubCos + x * Sin ,
					 x * z * OneSubCos + y * Sin , y * z * OneSubCos - x * Sin , z * z * OneSubCos + Cos );
}

Matrix3f Matrix3f::Identity()
{
	return Matrix3f();
}

Matrix3f Matrix3f::Tranpose( const Matrix3f & matrix )
{
	return Matrix3f( matrix.m00 , matrix.m10 , matrix.m20 ,
					 matrix.m01 , matrix.m11 , matrix.m21 ,
					 matrix.m02 , matrix.m12 , matrix.m22 );
}

Vector3f operator * ( const Vector3f& v , const Matrix3f& matrix )
{
	return Vector3f( v.x * matrix.m00 + v.y * matrix.m10 + v.z * matrix.m20 ,
					   v.x * matrix.m01 + v.y * matrix.m11 + v.z * matrix.m21 ,
					   v.x * matrix.m02 + v.y * matrix.m12 + v.z * matrix.m22 );
}

Vector3f Matrix3f::GetRow( int iRow ) const
{
	return Vector3f( m[iRow * 3 + 0] , m[iRow * 3 + 1] , m[iRow * 3 + 2] );
}

void Matrix3f::SetColumn( int iCol , const Vector3f& Vector )
{
	m[iCol] = Vector.x;
	m[iCol + 3] = Vector.y;
	m[iCol + 6] = Vector.z;
}

void Matrix3f::SetRow( int iRow , const Vector3f& Vector )
{
	m[iRow * 3 + 0] = Vector.x;
	m[iRow * 3 + 1] = Vector.y;
	m[iRow * 3 + 2] = Vector.z;
}

Vector3f Matrix3f::GetColumn( int iCol )
{
	return Vector3f( m[iCol] , m[iCol + 3] , m[iCol + 6] );
}