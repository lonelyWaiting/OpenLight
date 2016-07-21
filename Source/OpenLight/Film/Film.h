#pragma once

#include "Spectrum/Spectrum.h"
#include "Image.h"
#include "Math/Vector2.h"
#include "Core/VSerializableObject.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

class Film : public VSerializableObject
{
public:
	Film();

public:
	void SetFilename( char* filename );

	void SetGamma( float gamma );

	void SetResolution( int xResolution , int yResolution );

	void SetColor( int row , int col , Spectrum color );

	void Display();

	Image* GetImage();

public:
	Vector2f GetResolution() const;

	float GetAspectio() const;

public:
	void Deserialization( const tinyxml2::XMLElement* FilmRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	void Resize( int Width , int Height );

private:
	int Width;
	int Height;

	float Gamma;
	float InvGamma;

	char* Filename;

	Image* image;
};