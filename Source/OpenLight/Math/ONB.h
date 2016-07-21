#ifndef ONB_H
#define ONB_H

template<typename T>
class Vector3;

template<typename T>
class ONB
{
public:
	ONB();

	ONB( const Vector3<T>& u , const Vector3<T>& v , const Vector3<T>& w );

	void InitFromU( const Vector3<T>& u );
	void InitFromV( const Vector3<T>& v );
	void InitFromW( const Vector3<T>& w );

	void InitFromUV( const Vector3<T>& u , const Vector3<T>& v );

public:
	Vector3<T> U , V , W;
};

#include "ONB.inl"
#endif