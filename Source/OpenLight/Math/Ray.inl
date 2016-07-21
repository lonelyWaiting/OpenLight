template<typename T>
Ray<T>::Ray()
	: MinT( 0 )
	, MaxT( Infinity )
	, time( 0 )
	, depth( 0 )
{

}

template<typename T>
Ray<T>::Ray( const Point3<T> &_Origin , const Vector3<T> &_Direction , T _tMin , T _tMax /*= Infinity*/ , T _time /*= 0.0f*/ , int _depth /*= 0 */ )
	: Origin( _Origin )
	, Direction( _Direction )
	, MinT( _tMin )
	, MaxT( _tMax )
	, time( _time )
	, depth( _depth )
{

}

template<typename T>
Point3<T> Ray<T>::operator()( T t ) const
{
	return Origin + Direction * t;
}

template<typename T>
Ray<T>::Ray( const Point3<T>& Origin , const Vector3<T>& Direction , const Ray<T>& parent , T tmin , T tmax /*= Infinity*/ )
	: Origin( Origin )
	, Direction( Direction )
	, MinT( tmin )
	, MaxT( tmax )
	, time( parent.time )
	, depth( parent.depth + 1 )
{

}

template<typename T>
Ray<T>::Ray( const Ray<T>& ray )
{
	Origin = ray.Origin;
	Direction = ray.Direction;
	time = ray.time;
	MinT = ray.MinT;
	MaxT = ray.MaxT;
	depth = ray.depth;
}