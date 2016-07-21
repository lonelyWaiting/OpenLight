template<typename T>
Vector2<T>::Vector2()
{
	x = y = 0;
}

template<typename T>
Vector2<T>::Vector2( T _x , T _y )
	: x( _x )
	, y( _y )
{

}

template<typename T>
Vector2<T>::Vector2( const Point2<T> &p )
{
	x = p.x;
	y = p.y;
}

template<typename T>
Vector2<T>::Vector2( const Point3<T> &p )
{
	x = p.x;
	y = p.y;
}

template<typename T>
Vector2<T>::Vector2( const Vector2 &rhs )
{
	x = rhs.x;
	y = rhs.y;
}

template<typename T>
Vector2<T>& Vector2<T>::operator = ( const Vector2<T> &rhs )
{
	x = rhs.x;
	y = rhs.y;

	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator + ( const Vector2<T>& v ) const
{
	return Vector2( x + v.x , y + v.y );
}

template<typename T>
Vector2<T>& Vector2<T>::operator += ( const Vector2<T>& v )
{
	x += v.x;
	y += v.y;

	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator - ( const Vector2<T> &v ) const
{
	return Vector2<T>( x - v.x , y - v.y );
}

template<typename T>
Vector2<T>& Vector2<T>::operator -= ( const Vector2<T> &v )
{
	x -= v.x;
	y -= v.y;

	return *this;
}

template<typename T>
bool Vector2<T>::operator == ( const Vector2<T> &v ) const
{
	return ( x == v.x && y == v.y );
}

template<typename T>
bool Vector2<T>::operator != ( const Vector2<T> & v ) const
{
	return ( x != v.x || y != v.y );
}

template<typename T>
Vector2<T> Vector2<T>::operator* ( const Vector2<T>& v ) const
{
	return Vector2<T>( x * v.x , y * v.y );
}

template<typename T>
Vector2<T> Vector2<T>::operator* ( T f ) const
{
	return Vector2( f * x , f * y );
}

template<typename T>
Vector2<T>& Vector2<T>::operator *= ( T f )
{
	x *= f;
	y *= f;

	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator / ( T f ) const
{
	T inv = ( T )1 / f;
	return Vector2<T>( x * inv , y * inv );
}

template<typename T>
Vector2<T>& Vector2<T>::operator /= ( T f )
{
	T inv = ( T )1 / f;

	x *= inv;
	y *= inv;

	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator - () const
{
	return Vector2<T>( -x , -y );
}

template<typename T>
T& Vector2<T>::operator[] ( int index )
{
	return ( &x )[index];
}

template<typename T>
T Vector2<T>::LengthSq() const
{
	return x * x + y * y;
}

template<typename T>
T Vector2<T>::Length() const
{
	return std::sqrt( LengthSq() );
}

template<typename T>
Vector2<T> operator* ( T f , const Vector2<T> &v )
{
	return v * f;
}

template<typename T>
T Dot( const Vector2<T> &v1 , const Vector2<T> &v2 )
{
	return ( v1.x * v2.x + v1.y * v2.y );
}

template<typename T>
T AbsDot( const Vector2<T> &v1 , const Vector2<T> &v2 )
{
	return std::abs( Dot( v1 , v2 ) );
}

template<typename T>
Vector2<T> Normalize( const Vector2<T> &v )
{
	return v / v.Length();
}

template<typename T>
Vector2<T> Abs( const Vector2<T> &v )
{
	return Vector2<T>( std::abs( v.x ) , std::abs( v.y ) );
}