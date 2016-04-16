#pragma once

#include "Material.h"

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(Spectrum Reflection);

	BxDF* GetBxDF(const Point3f& point, const Normal& normal) const;

protected:
	BxDF* brdf;
};