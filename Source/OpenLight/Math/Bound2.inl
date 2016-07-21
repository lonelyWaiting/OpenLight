template<typename T>
Bound2<T>::Bound2()
{
	T MinNumber = ( std::numeric_limits<T>::lowest )( );
	T MaxNumber = ( std::numeric_limits<T>::max )( );

	pMin = Point2<T>( MaxNumber , MaxNumber );
	pMax = Point2<T>( MinNumber , MinNumber );
}

template<typename T>
Bound2<T>::Bound2( const Point2<T>& p )
	: pMin( p )
	, pMax( p )
{

}

template<typename T>
Bound2<T>::Bound2( const Point2<T>& p1 , const Point2<T>& p2 )
{
	pMin = Point2<T>( std::min( p1.x , p2.x ) , std::min( p1.y , p2.y ) );
	pMax = Point2<T>( std::max( p1.x , p2.x ) , std::max( p1.y , p2.y ) );
}

template<typename T>
Bound2<T>::operator Bound2<T>() const
{
	return Bound2<T>( pMin , pMax );
}

template<typename T>
Vector2<T> Bound2<T>::Diagonal() const
{
	return pMax - pMin;
}

template<typename T>
bool Bound2<T>::MaximumExtentIsWidth() const
{
	Vector2<T> diag = Diagonal();
	if( diag.x > diag.y )
	{
		return true;
	}

	return false;
}

template<typename T>
const Point2<T>& Bound2<T>::operator[]( int i ) const
{
	return ( i == 0 ? pMin : pMax );
}

template<typename T>
Point2<T>& Bound2<T>::operator[]( int i )
{
	return ( i == 0 ? pMin : pMax );
}

template<typename T>
bool Bound2<T>::operator == ( const Bound2<T>& rhs ) const
{
	return ( rhs.pMin == pMin && rhs.pMax == pMax );
}

template<typename T>
bool Bound2<T>::operator != ( const Bound2<T>& rhs ) const
{
	return ( rhs.pMin != pMin || rhs.pMax != pMax );
}

template<typename T>
Point2<T> Bound2<T>::Lerp( const Point2<T>& t ) const
{
	return Point2<T>( ::Lerp( t.x , pMin.x , pMax.y ) , ::Lerp( t.y , pMin.y , pMax.y ) );
}

template<typename T>
Vector2<T> Bound2<T>::Offset( const Point2<T>& p ) const
{
	Vector2<T> o = p - pMin;
	if( pMax.x > pMin.x )
	{
		o.x = o.x / ( pMax.x - pMin.x );
	}
	if( pMax.y > pMin.y )
	{
		o.y = o.y / ( pMax.y - pMin.y );
	}

	return o;
}

template<typename T>
void Bound2<T>::BoundingSphere( Point2<T>* Center , T *Radius ) const
{
	*Center = ( pMin + pMax ) * 0.5f;
	*Radius = Inside( *Center , *this ) ? ( pMax - pMin ).Length() * 0.5f : 0;
}

template<typename T>
bool Inside( const Point2<T>& pt , const Bound2<T>& b )
{
	return ( pt.x >= b.pMin.x && pt.x <= b.pMax.x && pt.y >= b.pMin.y && pt.y <= b.pMax.y );
}