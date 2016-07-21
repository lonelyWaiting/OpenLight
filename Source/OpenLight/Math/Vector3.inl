template<typename T>
Vector3<T>::Vector3() : x( 0 ) , y( 0 ) , z( 0 )
{

}

template<typename T>
Vector3<T>::Vector3( T value ) : x( value ) , y( value ) , z( value )
{

}

template<typename T>
Vector3<T>::Vector3( T _x , T _y , T _z ) : x( _x ) , y( _y ) , z( _z )
{

}

template<typename T>
Vector3<T>::Vector3( const Vector3<T>& rhs ) : x( rhs.x ) , y( rhs.y ) , z( rhs.z )
{

}

template<typename T>
Vector3<T>::Vector3( const Point3<T> &p )
{
	x = p.x;
	y = p.y;
	z = p.z;
}

template<typename T>
Vector3<T>& Vector3<T>::operator = ( const Vector3<T>& rhs )
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator + ( const Vector3<T>& rhs ) const
{
	return Vector3( x + rhs.x , y + rhs.y , z + rhs.z );
}

template<typename T>
Vector3<T>& Vector3<T>::operator += ( const Vector3<T>& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator - ( const Vector3<T>& rhs ) const
{
	return Vector3<T>( x - rhs.x , y - rhs.y , z - rhs.z );
}

template<typename T>
Vector3<T>& Vector3<T>::operator -= ( const Vector3<T>& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator * ( T scale ) const
{
	return Vector3( x * scale , y * scale , z * scale );
}

template<typename T>
Vector3<T>& Vector3<T>::operator *= ( T scale )
{
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}

template<typename T>
T Vector3<T>::operator * ( const Vector3<T> &rhs ) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

template<typename T>
Vector3<T> Vector3<T>::operator -() const
{
	return Vector3<T>( -x , -y , -z );
}

template<typename T>
Vector3<T> Vector3<T>::operator / ( T f ) const
{
	T inv = ( T )1 / f;

	return Vector3<T>( x * inv , y * inv , z * inv );
}

template<typename T>
Vector3<T>& Vector3<T>::operator /= ( T f )
{
	T inv = ( T )1 / f;

	x *= inv;
	y *= inv;
	z *= inv;

	return *this;
}

template<typename T>
T& Vector3<T>::operator [] ( int index )
{
	if( index == 0 ) return x;

	if( index == 1 ) return y;

	return z;
}

template<typename T>
T Vector3<T>::operator[] ( int index ) const
{
	if( index == 0 )	return x;

	if( index == 1 )	return y;

	return z;
}

template<typename T>
bool Vector3<T>::operator == ( const Vector3<T>& rhs ) const
{
	return ( x == rhs.x && y == rhs.y && z == rhs.z );
}

template<typename T>
bool Vector3<T>::operator != ( const Vector3<T>& rhs ) const
{
	return ( x != rhs.x || y != rhs.y || z != rhs.z );
}

template<typename T>
T Vector3<T>::LengthSq() const
{
	return ( x * x + y * y + z * z );
}

template<typename T>
T Vector3<T>::Length() const
{
	return std::sqrt( LengthSq() );
}

template<typename T>
Vector3<T> operator* ( T s , const Vector3<T> &v )
{
	return v * s;
}

template<typename T>
Vector3<T> Abs( const Vector3<T> &v )
{
	return Vector3<T>( std::abs( v.x ) , std::abs( v.y ) , std::abs( v.z ) );
}

template<typename T>
T Dot( const Vector3<T> &v1 , const Vector3<T> &v2 )
{
	return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z );
}

template<typename T>
T AbsDot( const Vector3<T> & v1 , const Vector3<T>& v2 )
{
	return std::abs( Dot( v1 , v2 ) );
}

template<typename T>
Vector3<T> Cross( const Vector3<T> &v1 , const Vector3<T> &v2 )
{
	T x1 = v1.x , y1 = v1.y , z1 = v1.z;
	T x2 = v2.x , y2 = v2.y , z2 = v2.z;

	return Vector3<T>( y1 * z2 - z1 * y2 , z1 * x2 - x1 * z2 , x1 * y2 - y1 * x2 );
}

template<typename T>
Vector3<T> Normalize( const Vector3<T>& v )
{
	return v / v.Length();
}

template<typename T>
T MinComponent( const Vector3<T> &v )
{
	return min( v.x , min( v.y , v.z ) );
}

template<typename T>
T MaxCompoent( const Vector3<T> &v )
{
	return max( v.x , max( v.y , v.z ) );
}

template<typename T>
Vector3<T> Max( const Vector3<T> &p1 , const Vector3<T> &p2 )
{
	return Vector3<T>( max( p1.x , p2.x ) , max( p1.y , p2.y ) , max( p1.z , p2.z ) );
}

template<typename T>
Vector3<T> Min( const Vector3<T> &p1 , const Vector3<T> &p2 )
{
	return Vector3<T>( min( p1.x , p2.x ) , min( p1.y , p2.y ) , min( p1.z , p2.z ) );
}

template<typename T>
Vector3<T> Permute( const Vector3<T> &v , int x , int y , int z )
{
	return Vector3<T>( v[x] , v[y] , v[z] );
}

template<typename T>
void CoordinateSystem( Vector3<T> &v1 , Vector3<T> *v2 , Vector3<T> *v3 )
{
	if( std::abs( v1.x ) > std::abs( v1.y ) )
	{
		*v2 = Vector3<T>( -v1.z , 0 , v1.x ) / std::sqrt( v1.x * v1.x + v1.z * v1.z );
	}
	else
	{
		*v2 = Vector3<T>( 0 , v1.z , -v1.y ) / std::sqrt( v1.y * v1.y + v1.z + v1.z );
	}

	*v3 = Cross( v1 , *v2 );
}

template<typename T>
int MaxLengthComponent( const Vector3<T>& v )
{
	T absX = std::abs( v.x );
	T absY = std::abs( v.y );
	T absZ = std::abs( v.z );

	if( absX > absY && absX > absZ )
	{
		return 0;
	}
	else if( absY > absZ )
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

template<typename T>
T Dot( const Vector3<T>& w , const Point3<T>& p )
{
	return w.x * p.x + w.y * p.y + w.z * p.z;
}