template<typename T>
Matrix3<T>::Matrix3()
	: m00( 1 ) , m01( 0 ) , m02( 0 )
	, m10( 0 ) , m11( 1 ) , m12( 0 )
	, m20( 0 ) , m21( 0 ) , m22( 1 )
{

}

template<typename T>
Matrix3<T>::Matrix3( const Matrix3<T>& Matrix )
	: m00( Matrix.m00 ) , m01( Matrix.m01 ) , m02( Matrix.m02 )
	, m10( Matrix.m10 ) , m11( Matrix.m11 ) , m12( Matrix.m12 )
	, m20( Matrix.m20 ) , m21( Matrix.m21 ) , m22( Matrix.m22 )
{

}


template<typename T>
Matrix3<T>::Matrix3( T fM00 , T fM01 , T fM02 ,
					 T fM10 , T fM11 , T fM12 ,
					 T fM20 , T fM21 , T fM22 )
	: m00( fM00 ) , m01( fM01 ) , m02( fM02 )
	, m10( fM10 ) , m11( fM11 ) , m12( fM12 )
	, m20( fM20 ) , m21( fM21 ) , m22( fM22 )
{

}

template<typename T>
Matrix3<T>& Matrix3<T>::operator = ( const Matrix3& Matrix )
{
	memcpy( this->m , Matrix.m , 9 * sizeof( T ) );

	return *this;
}

template<typename T>
T Matrix3<T>::operator[] ( int iPos ) const
{
	return m[iPos];
}

template<typename T>
T& Matrix3<T>::operator[] ( int iPos )
{
	return m[iPos];
}

template<typename T>
bool Matrix3<T>::operator == ( const Matrix3<T>& Matrix ) const
{
	return ( ( m00 == Matrix.m00 ) && ( m01 == Matrix.m01 ) && ( m02 == Matrix.m02 ) &&
			 ( m10 == Matrix.m10 ) && ( m11 == Matrix.m11 ) && ( m12 == Matrix.m12 ) &&
			 ( m20 == Matrix.m21 ) && ( m21 == Matrix.m21 ) && ( m22 == Matrix.m22 ) );
}

template<typename T>
bool Matrix3<T>::operator != ( const Matrix3<T>& Matrix ) const
{
	return ( ( m00 != Matrix.m00 ) || ( m01 != Matrix.m01 ) || ( m02 != Matrix.m02 ) ||
			 ( m10 != Matrix.m10 ) || ( m11 != Matrix.m11 ) || ( m12 != Matrix.m12 ) ||
			 ( m20 != Matrix.m21 ) || ( m21 != Matrix.m21 ) || ( m22 != Matrix.m22 ) );
}

template<typename T>
Matrix3<T> Matrix3<T>::operator * ( const Matrix3<T>& Matrix ) const
{
	return Matrix3<T>( m00 * Matrix[0] + m01 * Matrix[3] + m02 * Matrix[6] , m00 * Matrix[1] + m01 * Matrix[4] + m02 * Matrix[7] , m00 * Matrix[2] + m01 * Matrix[5] + m02 * Matrix[8] ,
					   m10 * Matrix[0] + m11 * Matrix[3] + m12 * Matrix[6] , m10 * Matrix[1] + m11 * Matrix[4] + m12 * Matrix[7] , m10 * Matrix[2] + m11 * Matrix[5] + m12 * Matrix[8] ,
					   m20 * Matrix[0] + m21 * Matrix[3] + m22 * Matrix[6] , m20 * Matrix[1] + m21 * Matrix[4] + m22 * Matrix[7] , m20 * Matrix[2] + m21 * Matrix[5] + m22 * Matrix[8] );
}

template<typename T>
Matrix3<T>& Matrix3<T>::operator *= ( const Matrix3<T>& rhs )
{
	Matrix3<T> temp = *this;

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

template<typename T>
void Matrix3<T>::Orthonormalize()
{
	// 使用施密特正交化方法
	T fInvLength = static_cast< T >( 1.0f / sqrt( m[0] * m[0] + m[3] * m[3] + m[6] * m[6] ) );

	m[0] *= fInvLength;
	m[3] *= fInvLength;
	m[6] *= fInvLength;

	T fDot0 = m[0] * m[1] + m[3] * m[4] + m[6] * m[7];

	m[1] -= fDot0 * m[0];
	m[4] -= fDot0 * m[3];
	m[7] -= fDot0 * m[6];

	fInvLength = static_cast< T >( 1.0f / sqrt( m[1] * m[1] + m[4] * m[4] + m[7] * m[7] ) );

	m[1] *= fInvLength;
	m[4] *= fInvLength;
	m[7] *= fInvLength;

	T fDot1 = m[0] * m[2] + m[3] * m[5] + m[6] * m[8];
	T fDot2 = m[1] * m[2] + m[4] * m[5] + m[7] * m[8];

	m[2] -= ( fDot1 * m[0] + fDot2 * m[1] );
	m[5] -= ( fDot1 * m[3] + fDot2 * m[4] );
	m[8] -= ( fDot1 * m[6] + fDot2 * m[7] );

	fInvLength = static_cast< T >( 1.0f / sqrt( m[2] * m[2] + m[5] * m[5] + m[8] * m[8] ) );

	m[2] *= fInvLength;
	m[5] *= fInvLength;
	m[8] *= fInvLength;
}

template<typename T>
Matrix3<T> Matrix3<T>::RotationX( T fTheta )
{
	T fSin = sin( ToRadians( fTheta ) );
	T fCos = cos( ToRadians( fTheta ) );

	return Matrix3<T>( 1.0f , 0.0f , 0.0f ,
					   0.0f , fCos , fSin ,
					   0.0f , -fSin , fCos );
}

template<typename T>
Matrix3<T> Matrix3<T>::RotationY( T fTheta )
{
	T fSin = sin( ToRadians( fTheta ) );
	T fCos = cos( ToRadians( fTheta ) );

	return Matrix3<T>( fCos , 0.0f , -fSin ,
					   0.0f , 1.0f , 0.0f ,
					   fSin , 0.0f , fCos );
}

template<typename T>
Matrix3<T> Matrix3<T>::RotationZ( T fTheta )
{
	T fSin = sin( ToRadians( fTheta ) );
	T fCos = cos( ToRadians( fTheta ) );

	return Matrix3<T>( fCos , fSin , 0.0f ,
					   -fSin , fCos , 0.0f ,
					   0.0f , 0.0f , 1.0f );
}

template<typename T>
Matrix3<T> Matrix3<T>::Rotation( T ThetaX , T ThetaY , T ThetaZ )
{
	T fSinX = sin( ToRadians( ThetaX ) );
	T fCosX = cos( ToRadians( ThetaX ) );
	T fSinY = sin( ToRadians( ThetaY ) );
	T fCosY = cos( ToRadians( ThetaY ) );
	T fSinZ = sin( ToRadians( ThetaZ ) );
	T fCosZ = cos( ToRadians( ThetaZ ) );

	T fCosYfCosZ = fCosY * fCosZ;
	T fSinYfCosZ = fSinY * fCosZ;
	T fSinXfCosZ = fSinX * fCosZ;
	T fSinXfSinZ = fSinX * fSinZ;

	return Matrix3<T>( fCosYfCosZ + fSinXfSinZ * fSinY , fCosX * fSinZ , -fSinYfCosZ + fSinXfSinZ * fCosY ,
					   -fCosY * fSinZ + fSinX * fSinYfCosZ , fCosX *fCosZ , fSinY * fSinZ + fCosYfCosZ * fSinX ,
					   fCosX * fSinY , -fSinX , fCosX * fCosY );
}

template<typename T>
Matrix3<T> Matrix3<T>::RotateAxis( Vector3<T> & Axis , T fTheta )
{
	T x = Axis.x;
	T y = Axis.y;
	T z = Axis.z;

	T Cos = cos( ToRadians( fTheta ) );
	T OneSubCos = 1 - Cos;
	T Sin = sin( ToRadians( fTheta ) );

	return Matrix3<T>( x * x * OneSubCos + Cos , x * y * OneSubCos + z * Sin , x * z * OneSubCos - y * Sin ,
					   x * y * OneSubCos - z * Sin , y * y * OneSubCos + Cos , y * z * OneSubCos + x * Sin ,
					   x * z * OneSubCos + y * Sin , y * z * OneSubCos - x * Sin , z * z * OneSubCos + Cos );
}

template<typename T>
Matrix3<T> Matrix3<T>::Identity()
{
	return Matrix3<T>();
}

template<typename T>
Matrix3<T> Matrix3<T>::Tranpose( const Matrix3<T> & matrix )
{
	return Matrix3<T>( matrix.m00 , matrix.m10 , matrix.m20 ,
					   matrix.m01 , matrix.m11 , matrix.m21 ,
					   matrix.m02 , matrix.m12 , matrix.m22 );
}

template<typename T>
Vector3<T> operator * ( const Vector3<T>& v , const Matrix3<T>& matrix )
{
	return Vector3<T>( v.x * matrix.m00 + v.y * matrix.m10 + v.z * matrix.m20 ,
					   v.x * matrix.m01 + v.y * matrix.m11 + v.z * matrix.m21 ,
					   v.x * matrix.m02 + v.y * matrix.m12 + v.z * matrix.m22 );
}

template<typename T>
Vector3<T> Matrix3<T>::GetRow( int iRow ) const
{
	return Vector3<T>( m[iRow * 3 + 0] , m[iRow * 3 + 1] , m[iRow * 3 + 2] );
}

template<typename T>
void Matrix3<T>::SetColumn( int iCol , const Vector3<T>& Vector )
{
	m[iCol] = Vector.x;
	m[iCol + 3] = Vector.y;
	m[iCol + 6] = Vector.z;
}

template<typename T>
void Matrix3<T>::SetRow( int iRow , const Vector3<T>& Vector )
{
	m[iRow * 3 + 0] = Vector.x;
	m[iRow * 3 + 1] = Vector.y;
	m[iRow * 3 + 2] = Vector.z;
}

template<typename T>
Vector3<T> Matrix3<T>::GetColumn( int iCol )
{
	return Vector3<T>( m[iCol] , m[iCol + 3] , m[iCol + 6] );
}