#pragma once

#include "Texture/Environment.h"

/*
	Format:
			<Environment type="ConstantEnvironment" color="0.0,0.0,0.0" />
*/
class ConstantEnvironment : public Environment
{
	DECLARE_DYNAMIC_CREATE_DERIVED( ConstantEnvironment , Environment )

public:
	ConstantEnvironment();

	ConstantEnvironment( Spectrum& color );

	Spectrum Evalute( Rayf& r );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	void SetBGColor( Spectrum& color );

private:
	Spectrum backgroundColor;
};