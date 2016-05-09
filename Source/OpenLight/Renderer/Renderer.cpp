#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "tinyxml2.h"
#include "Renderer.h"

Renderer::Renderer()
{

}

Renderer::Renderer( int _spp )
{
	spp = _spp;
}

Renderer::~Renderer()
{

}

void Renderer::SetSpp( int _spp )
{
	spp = _spp;
}

int Renderer::GetSpp() const
{
	return spp;
}