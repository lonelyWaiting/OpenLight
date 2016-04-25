#pragma once

#include "Spectrum/Spectrum.h"

class BSDF;
class Ray;
class Scene;
class Renderer;
class IntersectRecord;
class Accelerator;

Spectrum SpecularTransmit( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF );

Spectrum SpecularReflect( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF );