#pragma once

#include "Spectrum/Spectrum.h"

namespace tinyxml2
{
	class XMLElement;
}

class BSDF;
class Ray;
class Scene;
class Renderer;
struct IntersectRecord;
class Accelerator;
class Light;
class Point3f;
class Vector3f;
class Normal;
enum BxDFType;

Spectrum SpecularTransmit( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur );

Spectrum SpecularReflect( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur );

double PowerHeuristic( int nf , double fPdf , int ng , double gPdf );

Spectrum UniformSampleOneLight( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Normal& n );

Spectrum EstimateDirect( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , Light* light , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Normal& n , BxDFType flags );