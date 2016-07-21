template<typename T>
Matrix4<T>::Matrix4()
	: m00( 1 ) , m01( 0 ) , m02( 0 ) , m03( 0 )
	, m10( 0 ) , m11( 1 ) , m12( 0 ) , m13( 0 )
	, m20( 0 ) , m21( 0 ) , m22( 1 ) , m23( 0 )
	, m30( 0 ) , m31( 0 ) , m32( 0 ) , m33( 1 )
{

}

template<typename T>
Matrix4<T>::Matrix4( const T arr[4][4] )
{
	memcpy( m , arr , 16 * sizeof( T ) );
}

template<typename T>
Matrix4<T>::Matrix4( const T arr[16] )
{
	memcpy( m , arr , 16 * sizeof( T ) );
}

template<typename T>
Matrix4<T>::Matrix4( T _m00 , T _m01 , T _m02 , T _m03 ,
					 T _m10 , T _m11 , T _m12 , T _m13 ,
					 T _m20 , T _m21 , T _m22 , T _m23 ,
					 T _m30 , T _m31 , T _m32 , T _m33 )
{
	m00 = _m00;	m01 = _m01;	m02 = _m02;	m03 = _m03;

	m10 = _m10;	m11 = _m11;	m12 = _m12;	m13 = _m13;

	m20 = _m20;	m21 = _m21;	m22 = _m22; m23 = _m23;

	m30 = _m30;	m31 = _m31;	m32 = _m32;	m33 = _m33;
}

template<typename T>
Matrix4<T>::Matrix4( const Matrix4<T>& matrix )
{
	memcpy( m , matrix.m , 16 * sizeof( T ) );
}

template<typename T>
Matrix4<T>& Matrix4<T>::operator = ( const Matrix4<T>& rhs )
{
	memcpy( &m[0] , &( rhs.m[0] ) , 16 * sizeof( T ) );

	return ( *this );
}

template<typename T>
bool Matrix4<T>::operator == ( const Matrix4<T>& rhs ) const
{
	return ( ( m00 == rhs.m00 ) && ( m01 == rhs.m01 ) && ( m02 == rhs.m02 ) && ( m03 == rhs.m03 ) &&
			 ( m10 == rhs.m10 ) && ( m11 == rhs.m11 ) && ( m12 == rhs.m12 ) && ( m13 == rhs.m13 ) &&
			 ( m20 == rhs.m20 ) && ( m21 == rhs.m21 ) && ( m22 == rhs.m22 ) && ( m23 == rhs.m23 ) &&
			 ( m30 == rhs.m30 ) && ( m31 == rhs.m31 ) && ( m32 == rhs.m32 ) && ( m33 == rhs.m33 ) );
}

template<typename T>
bool Matrix4<T>::operator != ( const Matrix4<T>& rhs ) const
{
	return ( ( m00 != rhs.m00 ) || ( m01 != rhs.m01 ) || ( m02 != rhs.m02 ) || ( m03 != rhs.m03 ) ||
			 ( m10 != rhs.m10 ) || ( m11 != rhs.m11 ) || ( m12 != rhs.m12 ) || ( m13 != rhs.m13 ) ||
			 ( m20 != rhs.m20 ) || ( m21 != rhs.m21 ) || ( m22 != rhs.m22 ) || ( m23 != rhs.m23 ) ||
			 ( m30 != rhs.m30 ) || ( m31 != rhs.m31 ) || ( m32 != rhs.m32 ) || ( m33 != rhs.m33 ) );
}

template<typename T>
Matrix4<T> Matrix4<T>::operator * ( const Matrix4<T>& rhs ) const
{
	return Matrix4<T>( m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30 ,
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

template<typename T>
Matrix4<T> Matrix4<T>::Mul( const Matrix4<T>& lhs , const Matrix4<T>& rhs )
{
	return Matrix4<T>( lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30 ,
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

template<typename T>
Matrix4<T> Inverse( const Matrix4<T>& matrix )
{
	T fA0 = matrix.m[0] * matrix.m[5] - matrix.m[1] * matrix.m[4];
	T fA1 = matrix.m[0] * matrix.m[6] - matrix.m[2] * matrix.m[4];
	T fA2 = matrix.m[0] * matrix.m[7] - matrix.m[3] * matrix.m[4];
	T fA3 = matrix.m[1] * matrix.m[6] - matrix.m[2] * matrix.m[5];
	T fA4 = matrix.m[1] * matrix.m[7] - matrix.m[3] * matrix.m[5];
	T fA5 = matrix.m[2] * matrix.m[7] - matrix.m[3] * matrix.m[6];
	T fB0 = matrix.m[8] * matrix.m[13] - matrix.m[9] * matrix.m[12];
	T fB1 = matrix.m[8] * matrix.m[14] - matrix.m[10] * matrix.m[12];
	T fB2 = matrix.m[8] * matrix.m[15] - matrix.m[11] * matrix.m[12];
	T fB3 = matrix.m[9] * matrix.m[14] - matrix.m[10] * matrix.m[13];
	T fB4 = matrix.m[9] * matrix.m[15] - matrix.m[11] * matrix.m[13];
	T fB5 = matrix.m[10] * matrix.m[15] - matrix.m[14] * matrix.m[11];

	Matrix4<T> kInv;
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

	T fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;

	T fInvDet = ( ( T )1.0f ) / fDet;
	for( int i = 0; i < 16; i++ )
	{
		kInv.m[i] *= fInvDet;
	}

	return kInv;
}

template<typename T>
Matrix4<T> Transpose( const Matrix4<T>& rhs )
{
	Matrix4<T> Trans;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			Trans.m[iRow * 4 + iCol] = rhs.m[iCol * 4 + iRow];
		}
	}

	return Trans;
}

template<typename T>
void MakeTranspose( Matrix4<T>& rhs )
{
	Matrix4<T> Trans;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			Trans.m[iRow * 4 + iCol] = rhs.m[iCol * 4 + iRow];
		}
	}

	memcpy( &( rhs.m[0] ) , &( Trans.m[0] ) , 16 * sizeof( T ) );
}

template<typename T>
Matrix4<T> Matrix4<T>::Rotate( T fThetaX , T fThetaY , T fThetaZ )
{
	T sinThetaX = sin( ToRadians( fThetaX ) );
	T cosThetaX = cos( ToRadians( fThetaX ) );
	T sinThetaY = sin( ToRadians( fThetaY ) );
	T cosThetaY = cos( ToRadians( fThetaY ) );
	T sinThetaZ = sin( ToRadians( fThetaZ ) );
	T cosThetaZ = cos( ToRadians( fThetaZ ) );

	T cosYMulCosZ = cosThetaY * cosThetaZ;
	T sinYMulCosZ = sinThetaY * cosThetaZ;
	T sinXMulCosZ = sinThetaX * cosThetaZ;
	T sinXMulSinZ = sinThetaX * sinThetaZ;

	return Matrix4<T>( cosYMulCosZ + sinXMulSinZ * sinThetaY , cosThetaX * sinThetaZ , -sinYMulCosZ + sinXMulSinZ * cosThetaY , 0.0f ,
					   -cosThetaY * sinThetaZ + sinThetaX * sinYMulCosZ , cosThetaX *cosThetaZ , sinThetaY * sinThetaZ + cosYMulCosZ * sinThetaX , 0.0f ,
					   cosThetaX * sinThetaY , -sinThetaX , cosThetaX * cosThetaY , 0.0f ,
					   0.0f , 0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix4<T> Matrix4<T>::RotateAxis( const Vector3<T>& axis , T fTheta )
{
	Vector3<T> a = Normalize( axis );
	T x = a.x;
	T y = a.y;
	T z = a.z;

	T cosTheta = std::cos( ToRadians( fTheta ) );
	T OneSubCos = 1.0f - cosTheta;
	T sinTheta = std::sin( ToRadians( fTheta ) );

	return Matrix4<T>( x * x * OneSubCos + cosTheta , x * y * OneSubCos + z * sinTheta , x * z * OneSubCos - y * sinTheta , 0.0f ,
					   x * y * OneSubCos - z * sinTheta , y * y * OneSubCos + cosTheta , y * z * OneSubCos + x * sinTheta , 0.0f ,
					   x * z * OneSubCos + y * sinTheta , y * z * OneSubCos - x * sinTheta , z * z * OneSubCos + cosTheta , 0.0f ,
					   0.0f , 0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix4<T> Matrix4<T>::RotateX( T fTheta )
{
	T fSin = sin( ToRadians( fTheta ) );
	T fCos = cos( ToRadians( fTheta ) );

	return Matrix4<T>( 1.0f , 0.0f , 0.0f , 0.0f ,
					   0.0f , fCos , fSin , 0.0f ,
					   0.0f , -fSin , fCos , 0.0f ,
					   0.0f , 0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix4<T> Matrix4<T>::RotateY( T fTheta )
{
	T fSin = sin( ToRadians( fTheta ) );
	T fCos = cos( ToRadians( fTheta ) );

	return Matrix4<T>( fCos , 0.0f , -fSin , 0.0f ,
					   0.0f , 1.0f , 0.0f , 0.0f ,
					   fSin , 0.0f , fCos , 0.0f ,
					   0.0f , 0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix4<T> Matrix4<T>::RotateZ( T fTheta )
{
	T fSin = sin( ToRadians( fTheta ) );
	T fCos = cos( ToRadians( fTheta ) );

	return Matrix4<T>( fCos , fSin , 0.0f , 0.0f ,
					   -fSin , fCos , 0.0f , 0.0f ,
					   0.0f , 0.0f , 1.0f , 0.0f ,
					   0.0f , 0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix4<T> Matrix4<T>::Scale( T fScaleX , T fScaleY , T fScaleZ )
{
	return Matrix4<T>( fScaleX , 0.0f , 0.0f , 0.0f ,
					   0.0f , fScaleY , 0.0f , 0.0f ,
					   0.0f , 0.0f , fScaleZ , 0.0f ,
					   0.0f , 0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix4<T> Matrix4<T>::Translate( T fx , T fy , T fz )
{
	return Matrix4<T>( 1.0f , 0.0f , 0.0f , 0.0f ,
					   0.0f , 1.0f , 0.0f , 0.0f ,
					   0.0f , 0.0f , 1.0f , 0.0f ,
					   fx , fy , fz , 1.0f );
}

template<typename T>
Vector4<T> operator * ( const Vector4<T>& v , const Matrix4<T>& matrix )
{
	return Vector4<T>( v.x * matrix.m00 + v.y * matrix.m10 + v.z * matrix.m20 + v.w * matrix.m30 ,
					   v.x * matrix.m01 + v.y * matrix.m11 + v.z * matrix.m21 + v.w * matrix.m31 ,
					   v.x * matrix.m02 + v.y * matrix.m12 + v.z * matrix.m22 + v.w * matrix.m32 ,
					   v.x * matrix.m03 + v.y * matrix.m13 + v.z * matrix.m23 + v.w * matrix.m33 );
}

template<typename T>
Point3<T> operator * ( const Point3<T>& p , const Matrix4<T>& matrix )
{
	T w = p.x * matrix.m03 + p.y * matrix.m13 + p.z * matrix.m23 + matrix.m33;

	if( w == 1 )
	{
		return Point3<T>( p.x * matrix.m00 + p.y * matrix.m10 + p.z * matrix.m20 + matrix.m30 ,
						  p.x * matrix.m01 + p.y * matrix.m11 + p.z * matrix.m21 + matrix.m31 ,
						  p.x * matrix.m02 + p.y * matrix.m12 + p.z * matrix.m22 + matrix.m32 );
	}

	T inv = 1.0f / w;

	return Point3<T>( ( p.x * matrix.m00 + p.y * matrix.m10 + p.z * matrix.m20 + matrix.m30 ) * inv ,
					  ( p.x * matrix.m01 + p.y * matrix.m11 + p.z * matrix.m21 + matrix.m31 ) * inv ,
					  ( p.x * matrix.m02 + p.y * matrix.m12 + p.z * matrix.m22 + matrix.m32 ) * inv );
}

template<typename T>
Vector3<T> operator* ( const Vector3<T>& v , const Matrix4<T>& matrix )
{
	T x = v.x , y = v.y , z = v.z;

	// 向量没有位移
	return Vector3<T>( ( x * matrix.m00 + y * matrix.m10 + z * matrix.m20 ) ,
					   ( x * matrix.m01 + y * matrix.m11 + z * matrix.m21 ) ,
					   ( x * matrix.m02 + y * matrix.m12 + z * matrix.m22 ) );
}

template<typename T>
Matrix4<T> Matrix4<T>::LookAtLHMatrix( Point3<T>& eye , Point3<T>& at , Vector3<T>& up )
{
	Matrix4<T> ret;

	Vector3<T> zAxis = at - eye;
	zAxis = Normalize( zAxis );

	up = Normalize( up );
	Vector3<T> xAxis = Cross( up , zAxis );
	xAxis = Normalize( xAxis );

	Vector3<T> yAxis = Cross( zAxis , xAxis );

	ret.m[0] = xAxis.x;		ret.m[1] = yAxis.x;		ret.m[2] = zAxis.x;		ret.m[3] = 0.0f;
	ret.m[4] = xAxis.y;		ret.m[5] = yAxis.y;		ret.m[6] = zAxis.y;		ret.m[7] = 0.0f;
	ret.m[8] = xAxis.z;		ret.m[9] = yAxis.z;		ret.m[10] = zAxis.z;	ret.m[11] = 0.0f;

	ret.m[12] = -( Dot( xAxis , eye ) );

	ret.m[13] = -( Dot( yAxis , eye ) );

	ret.m[14] = -( Dot( zAxis , eye ) );

	ret.m[15] = 1.0f;

	return ret;
}

template<typename T>
void Matrix4<T>::SetRow( int i , const Vector3<T>& v )
{
	m[4 * i + 0] = v.x;
	m[4 * i + 1] = v.y;
	m[4 * i + 2] = v.z;
}

template<typename T>
void Matrix4<T>::SetColumn( int i , const Vector3<T>& v )
{
	m[i] = v.x;
	m[4 + i] = v.y;
	m[8 + i] = v.z;
}


template<typename T>
void Matrix4<T>::SetTranslate( const Vector3<T>& v )
{
	m[12] = v.x;
	m[13] = v.y;
	m[14] = v.z;
}