#pragma once

class Filter
{
public:
	virtual ~Filter();

	Filter( float xWidth , float yWidth )
		: xWidth( xWidth )
		, yWidth( yWidth )
		, xInvWidth( 1.0f / xWidth )
		, yInvWidth( 1.0f / yWidth )
	{

	}

	virtual float Evalute( float x , float y ) const = 0;

public:
	const float xWidth;
	const float yWidth;
	const float xInvWidth;
	const float yInvWidth;
};