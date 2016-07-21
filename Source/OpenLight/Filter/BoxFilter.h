#pragma once

#include "Filter/Filter.h"

class BoxFilter : public Filter
{
public:
	BoxFilter( float xw , float yw )
		:Filter( xw , yw )
	{

	}

	float Evalute( float x , float y ) const;
};

BoxFilter* CreateBoxFilter()
{
	// д╛хо
	return new BoxFilter( 0.5f , 0.5f );
}