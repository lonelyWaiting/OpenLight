#pragma once

#include "Filter/Filter.h"

class BoxFilter : public Filter
{
public:
	BoxFilter( double xw , double yw )
		:Filter( xw , yw )
	{

	}

	double Evalute( double x , double y ) const;
};

BoxFilter* CreateBoxFilter()
{
	// д╛хо
	return new BoxFilter( 0.5f , 0.5f );
}