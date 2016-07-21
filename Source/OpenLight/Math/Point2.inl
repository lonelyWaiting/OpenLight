template<typename T>
Point2<T>::Point2()
{
	x = y = 0;
}

template<typename T>
Point2<T>::Point2( T _x , T _y )
{
	x = _x;
	y = _y;
}

template<typename T>
Point2<T>::Point2( const Point2<T> &p )
{
	x = p.x;
	y = p.y;
}

template<typename T>
Point2<T>::Point2( const Vector2<T> &p )
{
	x = p.x;
	y = p.y;
}

template<typename T>
Point2<T>& Point2<T>::operator = ( const Point2<T> &p )
{
	x = p.x;
	y = p.y;

	return ( *this );
}

template<typename T>
Point2<T> Point2<T>::operator + ( const Vector2<T> &v ) const
{
	return Point2<T>( x + v.x , y + v.y );
}

template<typename T>
Point2<T>& Point2<T>::operator += ( const Vector2<T> &v )
{
	x += v.x;
	y += v.y;

	return *this;
}

template<typename T>
Point2<T> Point2<T>::operator + ( const Point2<T> &p ) const
{
	return Point2<T>( x + p.x , y + p.y );
}

template<typename T>
Vector2<T> Point2<T>::operator - ( const Point2<T> & p ) const
{
	return Vector2<T>( x - p.x , y - p.y );
}

template<typename T>
Point2<T> Point2<T>::operator- ( const Vector2<T> &v ) const
{
	return Point2<T>( x - v.x , y - v.y );
}

template<typename T>
Point2<T> Point2<T>::operator - () const
{
	return Point2<T>( -x , -y );
}

template<typename T>
Point2<T>& Point2<T>::operator-= ( const Vector2<T> &v )
{
	x -= v.x;
	y -= v.y;

	return ( *this );
}

template<typename T>
Point2<T> Point2<T>::operator * ( T f ) const
{
	return Point2<T>( f * x , f * y );
}

template<typename T>
Point2<T>& Point2<T>::operator *= ( T f )
{
	x *= f;
	y *= f;

	return ( *this );
}

template<typename T>
Point2<T> Point2<T>::operator / ( T f ) const
{
	T inv = ( T )1 / f;

	return Point2<T>( inv * x , inv * y );
}

template<typename T>
Point2<T> Point2<T>::operator /= ( T f )
{
	T inv = ( T )1 / f;

	x *= inv;
	y *= inv;

	return *this;
}

template<typename T>
T Point2<T>::operator[] ( int index ) const
{
	if( index == 0 )	return x;
	return y;
}

template<typename T>
T& Point2<T>::operator[] ( int index )
{
	if( index == 0 )	return x;
	return y;
}

template<typename T>
bool Point2<T>::operator == ( const Point2<T> &p ) const
{
	return ( x == p.x && y == p.y );
}

template<typename T>
bool Point2<T>::operator != ( const Point2<T> &p ) const
{
	return ( x != p.x || y != p.y );
}

template<typename T>
T Distance( Point2<T>& p1 , Point2<T>& p2 )
{
	return ( p1 - p2 ).Length();
}

template<typename T>
T DistanceSq( const Point2<T> &p1 , const Point2<T> &p2 )
{
	return ( p1 - p2 ).LengthSq();
}

template<typename T>
Point2<T> operator* ( T f , const Point2<T> &p )
{
	return p * f;
}

template<typename T>
Point2<T> Floor( const Point2<T> &p )
{
	return Point2<T>( floor( p.x ) , floor( p.y ) );
}

template<typename T>
Point2<T> Ceil( const Point2<T> &p )
{
	return Point2<T>( ceil( p.x ) , ceil( p.y ) );
}

template<typename T>
Point2<T> Lerp( T t , const Point2<T> &p0 , const Point2<T> &p1 )
{
	return ( 1 - t ) * p0 + t * p1;
}

template<typename T>
Point2<T> Min( const Point2<T> &p0 , const Point2<T> &p1 )
{
	return Point2<T>( min( p0.x , p1.x ) , min( p0.y , p1.y ) );
}

template<typename T>
Point2<T> Max( const Point2<T> &p0 , const Point2<T> &p1 )
{
	return Point2<T>( max( p0.x , p1.x ) , max( p0.y , p1.y ) );
}