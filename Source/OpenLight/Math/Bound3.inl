template<typename T>
Bound3<T>::Bound3()
{
	T MinNumber = ( std::numeric_limits<T>::lowest )( );
	T MaxNumber = ( std::numeric_limits<T>::max )( );

	pMin = Point3<T>( MaxNumber , MaxNumber , MaxNumber );
	pMax = Point3<T>( MinNumber , MinNumber , MinNumber );
}

template<typename T>
Bound3<T>::Bound3( const Point3<T>& p )
	:pMin( p )
	, pMax( p )
{

}

template<typename T>
Bound3<T>::Bound3( const Point3<T>& p1 , const Point3<T>& p2 )
	:pMin( std::min( p1.x , p2.x ) , std::min( p1.y , p2.y ) , std::min( p1.z , p2.z ) )
	, pMax( std::max( p1.x , p2.x ) , std::max( p1.y , p2.y ) , std::max( p1.z , p2.z ) )
{

}

template<typename T>
const Point3<T>& Bound3<T>::operator[]( int i ) const
{
	return i == 0 ? pMin : pMax;
}

template<typename T>
Point3<T>& Bound3<T>::operator[]( int i )
{
	return i == 0 ? pMin : pMax;
}

template<typename T>
Point3<T> Bound3<T>::Corner( int corner ) const
{
	return Point3<T>( ( *this )[( corner & 1 )].x , ( *this )[( corner & 2 ) ? 1 : 0].y , ( *this )[( corner & 4 ) ? 1 : 0].z );
}

template<typename T>
Vector3<T> Bound3<T>::Diagonal() const
{
	return pMax - pMin;
}

template<typename T>
T Bound3<T>::SurfaceArea() const
{
	Vector3<T> d = Diagonal();

	return 2 * ( d.x * d.y + d.x * d.z + d.y * d.z );
}

template<typename T>
Vector3<T> Bound3<T>::Offset( const Point3<T>& p ) const
{
	Vector3<T> o = p - pMin;
	if( pMax.x > pMin.x )	o.x /= pMax.x - pMin.x;
	if( pMax.y > pMin.y )	o.y /= pMax.y - pMin.y;
	if( pMax.z > pMin.z )	o.z /= pMax.z - pMin.z;

	return o;
}

template<typename T>
void Bound3<T>::BoundingSphere( Point3<T>* center , T* radius ) const
{
	*center = ( pMin + pMax ) * 0.5;
	*radius = Inside( *center , *this ) ? ( pMax - pMin ).Length() / 2 : 0;
}

template<typename T>
void Bound3<T>::ExpendToInclude( const Point3<T>& p )
{
	pMin.x = std::min( pMin.x , p.x );
	pMin.y = std::min( pMin.y , p.y );
	pMin.z = std::min( pMin.z , p.z );

	pMax.x = std::max( pMax.x , p.x );
	pMax.y = std::max( pMax.y , p.y );
	pMax.z = std::max( pMax.z , p.z );
}

template<typename T>
void Bound3<T>::ExpendToInclude( const Bound3<T>& bbox )
{
	pMin.x = std::min( pMin.x , bbox.pMin.x );
	pMin.y = std::min( pMin.y , bbox.pMin.y );
	pMin.z = std::min( pMin.z , bbox.pMin.z );

	pMax.x = std::max( pMax.x , bbox.pMax.x );
	pMax.y = std::max( pMax.y , bbox.pMax.y );
	pMax.z = std::max( pMax.z , bbox.pMax.z );
}

template<typename T>
bool Inside( const Point3<T>& pt , const Bound3<T>& b )
{
	return ( pt.x >= b.pMin.x && pt.x <= b.pMax.x && pt.y >= b.pMin.y && pt.y <= b.pMax.y && pt.z >= b.pMin.z && pt.z <= b.pMax.z );
}

template<typename T>
bool Bound3<T>::IntersectP( const Ray<T>& ray , T* hitt0 /* = nullptr  */ , T* hitt1 /* = nullptr */ ) const
{
	T t0 = 0 , t1 = ray.MaxT;

	for( int i = 0; i < 3; i++ )
	{
		T invRayDir = 1 / ray.Direction[i];

		T tNear = ( pMin[i] - ray.Origin[i] ) * invRayDir;
		T tFar = ( pMax[i] - ray.Origin[i] ) * invRayDir;

		if( tNear > tFar )
		{
			std::swap( tNear , tFar );
		}

		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;

		if( t0 > t1 )
		{
			return false;
		}
	}

	if( hitt0 )
	{
		*hitt0 = t0;
	}

	if( hitt1 )
	{
		*hitt1 = t1;
	}

	return true;
}

template<typename T>
bool Bound3<T>::IntersectP( const Ray<T> & ray , const Vector3<T> & invDir , const int DirectionIsNegative[3] )
{
	return false;
}