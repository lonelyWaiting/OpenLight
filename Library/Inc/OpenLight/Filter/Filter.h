#pragma once

class Filter
{
public:
	virtual ~Filter();

	Filter( double xWidth , double yWidth )
		: xWidth( xWidth )
		, yWidth( yWidth )
		, xInvWidth( 1.0f / xWidth )
		, yInvWidth( 1.0f / yWidth )
	{

	}

	virtual double Evalute( double x , double y ) const = 0;

public:
	const double xWidth;
	const double yWidth;
	const double xInvWidth;
	const double yInvWidth;
};