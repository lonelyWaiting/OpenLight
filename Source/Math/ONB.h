#pragma once

#include "Vector3.h"

class ONB
{
public:
	ONB();

	ONB( const Vector3f& u , const Vector3f& v , const Vector3f& w );

	void InitFromU( const Vector3f& u );
	void InitFromV( const Vector3f& v );
	void InitFromW( const Vector3f& w );

	void InitFromUV( const Vector3f& u , const Vector3f& v );

public:
	Vector3f U , V , W;
};