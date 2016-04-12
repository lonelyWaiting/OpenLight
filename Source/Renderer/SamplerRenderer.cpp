#include "PCH.h"
#include "Spectrum/Spectrum.h"
#include "Primitive/IntersectRecord.h"
#include "Core/Scene.h"
#include "Math/Vector2.h"
#include "Camera/Camera.h"
#include "Sampler/Sampler.h"
#include "Core/Scene.h"
#include "Integrator/SurfaceIntegrator.h"
#include "SamplerRenderer.h"

SamplerRenderer::SamplerRenderer( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator )
	: sampler( _sampler )
	, camera( _camera )
	, surfaceIntegrator( _surfaceIntegrator )
{

}

void SamplerRenderer::Render( const Scene* scene )
{
	Vector2f Resolution = camera->GetFilm()->GetResolution();
	
	int Width  = Resolution.x;
	int Height = Resolution.y;

#pragma omp parallel for schedule(dynamic)
	for( int iRow = 0; iRow < Height; iRow++ )
	{
		for( int iCol = 0; iCol < Width; iCol++ )
		{
			Spectrum L( 0 );

			for( int i = 0; i < 64; i++ )
			{
				CameraSample SamplePoint = sampler->GetSamplePoint();

				Ray ray = camera->GenerateRay( iCol , iRow , SamplePoint );

				IntersectRecord record;

				Spectrum T( 1.0f );

				L += Li( scene , &ray , &record , &T );
			}
			
			L /= 64;

			camera->GetFilm()->SetColor( iRow , iCol , L );
		}
	}

	camera->GetFilm()->Display();
}

Spectrum SamplerRenderer::Li( const Scene* scene , Ray* ray , IntersectRecord* record /* = nullptr  */ , Spectrum* T /* = nullptr */ ) const
{
	Spectrum LocalT;

	if( T == nullptr )
	{
		T = &LocalT;
	}

	IntersectRecord LocalIntersectRecord;

	if( record == nullptr )
	{
		record = &LocalIntersectRecord;
	}

	Spectrum Li = 0.0f;

	// 检测是否有图元与之相交
	if( scene->Intersect( *ray , record ) )
	{
		Li = surfaceIntegrator->Li( scene , this , record , ray );
	}
	else
	{
		for( uint32_t i = 0; i < scene->GetLights().size(); i++ )
		{
			Li += ( scene->GetLights() )[i]->Le( *ray );
		}
	}

	return ( ( *T ) * Li );
}

Spectrum SamplerRenderer::Transmittance( const Scene* scene , const Sampler* sample , RandomNumberGenerator& rng ) const
{
	return Spectrum( 0 );
}