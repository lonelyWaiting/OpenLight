#ifndef OPEN_LIGHT_H
#define OPEN_LIGHT_H

#include <Core/Scene.h>

#include <Math/Transform.h>
#include <Math/Point3.h>

#include <Camera/ThinLensCamera.h>
#include <Camera/PinholeCamera.h>

#include <Renderer/SamplerRenderer.h>

#include <Primitive/Primitive.h>

#include <Sampler/PureRandomSampler.h>
#include <Sampler/NRooksSampler.h>

#include <Spectrum/Spectrum.h>

#include <Utilities/srString.h>

#include <IO/FileSystem.h>

#include <Sampler/NRooksSampler.h>

#include <BRDF/Lambertian.h>

#include <Light/PointLight.h>

#include <Accelerator/Grid.h>

#include <Integrator/WhittedIntegrator.h>
#include <Integrator/DirectIntegrator.h>
#include <Integrator/PathIntegrator.h>

#include <Shapes/Sphere.h>
#include <Shapes/Triangle.h>
#include <Shapes/TriangleMesh.h>

#include <Material/DiffuseMaterial.h>
#include <Material/GlassMaterial.h>
#include <Material/PureReflectionMaterial.h>

#include <Light/PointLight.h>
#include <Light/AreaLighth.h>

#include <Texture/ConstantTexture.h>
#include <Texture/CheckboardTexture.h>
#include <Texture/ImageTexture.h>

#include <Texture/ConstantEnvironment.h>
#include <Texture/HDREnvironment.h>

namespace OpenLight
{
	extern Scene scene;
};

#endif