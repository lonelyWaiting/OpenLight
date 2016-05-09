#pragma once

#include "Spectrum/Spectrum.h"

class BSDF;
class Ray;
class Scene;
class Renderer;
struct IntersectRecord;
class Accelerator;

Spectrum SpecularTransmit( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur );

Spectrum SpecularReflect( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur );