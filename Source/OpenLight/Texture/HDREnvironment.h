#pragma once

#include "Environment.h"

class ImageFile;

/*
	Format:
			<Environment type="HDREnvironment" filename="bg.pfm">
*/
class HDREnvironment : public Environment
{
	DECLARE_DYNAMIC_CREATE_DERIVED( HDREnvironment , Environment )

public:
	HDREnvironment();

	~HDREnvironment();

	Spectrum Evalute( Rayf& r );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	char*      m_Filename;
	ImageFile* m_pImage;
};