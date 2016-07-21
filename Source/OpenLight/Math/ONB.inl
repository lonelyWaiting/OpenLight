#define ONB_EPSILON 0.01f

template<typename T>
ONB<T>::ONB()
{

}

template<typename T>
ONB<T>::ONB( const Vector3<T>& u , const Vector3<T>& v , const Vector3<T>& w )
{
	U = u;
	V = v;
	W = w;
}

template<typename T>
void ONB<T>::InitFromU( const Vector3<T>& u )
{
	Vector3<T> n( 1.0f , 0.0f , 0.0f );
	Vector3<T> m( 0.0f , 1.0f , 0.0f );

	U = Normalize( u );
	W = Cross( U , m );
	if( W.Length() < ONB_EPSILON )		// U , m½üºõÆ½ÐÐ
	{
		W = Cross( n , U );
		V = Cross( W , U );
	}
	else
	{
		V = Cross( W , U );
	}

	Normalize( W );
	Normalize( V );
}

template<typename T>
void ONB<T>::InitFromV( const Vector3<T>& v )
{
	Vector3<T> z( 0.0f , 0.0f , 1.0f );
	Vector3<T> x( 1.0f , 0.0f , 0.0f );

	V = Normalize( v );
	U = Cross( V , z );
	if( U.Length() < ONB_EPSILON )
	{
		W = Cross( x , V );
		U = Cross( V , W );
	}
	else
	{
		W = Cross( U , V );
	}

	Normalize( W );
	Normalize( U );
}

template<typename T>
void ONB<T>::InitFromW( const Vector3<T>& w )
{
	Vector3<T> x( 1.0f , 0.0f , 0.0f );
	Vector3<T> y( 0.0f , 1.0f , 0.0f );

	W = Normalize( w );
	U = Cross( y , W );

	if( U.Length() < ONB_EPSILON )
	{
		V = Cross( x , W );
		U = Cross( V , W );
	}
	else
	{
		V = Cross( W , U );
	}
	
	Normalize( U );
	Normalize( V );
}

template<typename T>
void ONB<T>::InitFromUV( const Vector3<T>& u , const Vector3<T>& v )
{
	U = Normalize( u );
	W = Normalize( Cross( u , v ) );
	V = Cross( W , U );
}

template<typename T>
bool operator == ( const ONB<T>& lhs , const ONB<T>& rhs )
{
	return ( lhs.U == rhs.U ) && ( lhs.V == rhs.V ) && ( lhs.W == rhs.W );
}