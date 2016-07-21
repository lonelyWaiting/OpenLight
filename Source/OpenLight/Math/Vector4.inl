template<typename T>
Vector4<T>::Vector4() :x( T( 0 ) ) , y( T( 0 ) ) , z( T( 0 ) ) , w( T( 0 ) )
{

}

template<typename T>
Vector4<T>::Vector4( T a ) : x( a ) , y( a ) , z( a ) , w( a )
{

}

template<typename T>
Vector4<T>::Vector4( T x , T y , T z , T w ) :x( x ) , y( y ) , z( z ) , w( w )
{

}

template<typename T>
Vector4<T>::Vector4( const Vector4<T>& v ) : x( v.x ) , y( v.y ) , z( v.z ) , w( v.w )
{

}

template<typename T>
Vector4<T>::Vector4( const Vector3<T>& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 1.0f;
}

template<typename T>
bool Vector4<T>::operator == ( const Vector4<T>& v ) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

template<typename T>
bool Vector4<T>::operator != ( const Vector4<T>& v ) const
{
	return ( x != v.x ) || ( y != v.y ) || ( z != v.z ) || ( w != v.w );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator = ( const Vector4<T>& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;

	return ( *this );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator += ( const Vector4<T>& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

template<typename T>
Vector4<T> Vector4<T>::operator + ( const Vector4<T>& v ) const
{
	return Vector4<T>( x + v.x , y + v.y , z + v.z , w + v.w );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator -= ( const Vector4<T>& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return ( *this );
}

template<typename T>
Vector4<T> Vector4<T>::operator - ( const Vector4<T>& v ) const
{
	return Vector4<T>( x - v.x , y - v.y , z - v.z , w - v.w );
}

template<typename T>
Vector4<T> Vector4<T>::operator - () const
{
	return Vector4<T>( -x , -y , -z , -w );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator *= ( const Vector4<T>& v )
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return ( *this );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator *= ( T a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return ( *this );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator /= ( const Vector4<T>& v )
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return ( *this );
}

template<typename T>
const Vector4<T>& Vector4<T>::operator /= ( T a )
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;

	return ( *this );
}

template<typename T>
Vector4<T> Vector4<T>::operator / ( const Vector4<T>& v ) const
{
	return Vector4<T>( x / v.x , y / v.y , z / v.z , w / v.w );
}

template<typename T>
Vector4<T> Vector4<T>::operator / ( T a ) const
{
	return Vector4<T>( x / a , y / a , z / a , w / a );
}

template<typename T>
T Vector4<T>::length() const
{
	return std::sqrt( x * x + y * y + z * z + w * w );
}

template<typename T>
T Vector4<T>::lengthSq() const
{
	return x * x + y * y + z * z + w * w;
}

template<typename T>
const Vector4<T>& Vector4<T>::normalize()
{
	T l = length();

	if( l != T( 0 ) )
	{
		x /= l;
		y /= l;
		z /= l;
		w /= l;
	}

	return *this;
}

template<typename T>
unsigned int Vector4<T>::dimension()
{
	return 4;
}

template<typename T>
T Dot( const Vector4<T> &v1 , const Vector4<T> &v2 )
{
	return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w );
}

template<typename T>
T AbsDot( const Vector4<T>& v1 , const Vector4<T>& v2 )
{
	return std::abs( Dot( v1 , v2 ) );
}

template<typename T>
Vector4<T> Abs( const Vector4<T>& v )
{
	return Vector4<T>( std::abs( v.x ) , std::abs( v.y ) , std::abs( v.z ) , std::abs( v.w ) );
}

template<typename T>
Vector4<T> Permute( const Vector4<T> &v , int x , int y , int z , int w )
{
	return Vector4<T>( ( &v.x )[x] , ( &v.x )[y] , ( &v.x )[z] , ( &v.x )[w] );
}

template<typename T>
Vector4<T> Normalize( const Vector4<T>& v )
{
	return v / v.length();
}