template<typename T>
Point3<T>::Point3()
{
	x = y = z = 0;
}

template<typename T>
Point3<T>::Point3( T _x , T _y , T _z )
{
	x = _x;
	y = _y;
	z = _z;
}

template<typename T>
Point3<T>::Point3( const Point3<T> &p )
{
	x = p.x;
	y = p.y;
	z = p.z;
}

template<typename T>
Point3<T>& Point3<T>::operator = ( const Point3<T>& p )
{
	x = p.x;
	y = p.y;
	z = p.z;

	return *this;
}

template<typename T>
Point3<T> Point3<T>::operator + ( const Vector3<T>& v ) const
{
	return Point3( x + v.x , y + v.y , z + v.z );
}

template<typename T>
Point3<T>& Point3<T>::operator += ( const Vector3<T>& v )
{
	x += v.x;
	y += v.y;
	z += v.z;

	return ( *this );
}

template<typename T>
Vector3<T> Point3<T>::operator - ( const Point3<T>& p ) const
{
	return Vector3<T>( x - p.x , y - p.y , z - p.z );
}

template<typename T>
Point3<T> Point3<T>::operator - ( const Vector3<T> &v ) const
{
	return Point3<T>( x - v.x , y - v.y , z - v.z );
}

template<typename T>
Point3<T>& Point3<T>::operator -= ( const Vector3<T> &p )
{
	x -= p.x;
	y -= p.y;
	z -= p.z;

	return ( *this );
}

template<typename T>
Point3<T> Point3<T>::operator + ( const Point3<T> &p ) const
{
	return Point3<T>( x + p.x , y + p.y , z + p.z );
}

template<typename T>
Point3<T> Point3<T>::operator* ( T f ) const
{
	return Point3<T>( f * x , f * y , f * z );
}

template<typename T>
Point3<T>& Point3<T>::operator *= ( T f )
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

template<typename T>
Point3<T> Point3<T>::operator / ( T f ) const
{
	T inv = ( T )1 / f;

	return Point3<T>( x * inv , y * inv , z * inv );
}

template<typename T>
Point3<T>& Point3<T>::operator /= ( T f )
{
	T inv = 1.0f / f;

	x *= inv;
	y *= inv;
	z *= inv;

	return ( *this );
}

template<typename T>
T Point3<T>::operator[] ( int index ) const
{
	if( index == 0 )	return x;

	if( index == 1 )	return y;

	return z;
}

template<typename T>
T& Point3<T>::operator [] ( int i )
{
	if( i == 0 )	return x;

	if( i == 1 )	return y;

	return z;
}

template<typename T>
bool Point3<T>::operator == ( const Point3<T>& p ) const
{
	return ( x == p.x && y == p.y && z == p.z );
}

template<typename T>
bool Point3<T>::operator != ( const Point3<T>& p ) const
{
	return ( x != p.x || y != p.y || z != p.z );
}

template<typename T>
T Distance( const Point3<T> &p1 , const Point3<T> &p2 )
{
	return ( p1 - p2 ).Length();
}

template<typename T>
T DistanceSq( const Point3<T> &p1 , const Point3<T> &p2 )
{
	return ( p1 - p2 ).LengthSq();
}

template<typename T>
Point3<T> operator*( T f , const Point3<T> &p )
{
	return p * f;
}

template<typename T>
Point3<T> Lerp( T t , const Point3<T> &p0 , const Point3<T>& p1 )
{
	return ( 1 - t ) * p0 + t * p1;
}

template<typename T>
Point3<T> Min( const Point3<T> &p1 , const Point3<T> &p2 )
{
	return Point3<T>( min( p1.x , p2.x ) , min( p1.y , p2.y ) , min( p1.z , p2.z ) );
}

template<typename T>
Point3<T> Max( const Point3<T> &p1 , const Point3<T> &p2 )
{
	return Point3<T>( max( p1.x , p2.x ) , max( p1.y , p1.z ) , max( p1.z , p2.z ) );
}

template<typename T>
Point3<T> Floor( const Point3<T> &p )
{
	return Point3<T>( floor( p.x ) , floor( p.y ) , floor( p.z ) );
}

template<typename T>
Point3<T> Ceil( const Point3<T> &p )
{
	return Point3<T>( ceil( p.x ) , ceil( p.y ) , ceil( p.z ) );
}

template<typename T>
Point3<T> Abs( const Point3<T>& p )
{
	return Point3<T>( std::abs( p.x ) , std::abs( p.y ) , std::abs( p.z ) );
}

template<typename T>
Point3<T> Permute( const Point3<T> &p , int x , int y , int z )
{
	return Point3<T>( p.x , p.y , p.z );
}

template<typename T>
T operator*( const Point3<T>& p , const Vector3<T>& v )
{
	return p.x * v.x + p.y * v.y + p.z * v.z;
}