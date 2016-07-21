#pragma once

#include <Spectrum/Spectrum.h>

namespace tinyxml2
{
	class XMLElement;
}

class BSDF;
class Scene;
class Renderer;
struct IntersectRecord;
class Accelerator;
class Light;
enum BxDFType;

Spectrum SpecularTransmit( const Rayf& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur );

Spectrum SpecularReflect( const Rayf& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur );

float PowerHeuristic( int nf , float fPdf , int ng , float gPdf );

Spectrum UniformSampleOneLight( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Vector3f& n );

Spectrum EstimateDirect( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , Light* light , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Vector3f& n , BxDFType flags );