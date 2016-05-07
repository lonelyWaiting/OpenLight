#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "Primitive/IntersectRecord.h"
#include "Core/Scene.h"
#include "Math/Vector2.h"
#include "Camera/Camera.h"
#include "Sampler/Sampler.h"
#include "Core/Scene.h"
#include "Integrator/SurfaceIntegrator.h"
#include "Accelerator/Grid.h"
#include "tinyxml2.h"
#include "SamplerRenderer.h"

SamplerRenderer::SamplerRenderer()
	:Renderer()
{

}

SamplerRenderer::SamplerRenderer( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator , int _spp/*=8*/ )
	: Renderer( _spp )
	, sampler( _sampler )
	, camera( _camera )
	, surfaceIntegrator( _surfaceIntegrator )
	, pAccelerator( _pAccelerator )
{

}

SamplerRenderer::~SamplerRenderer()
{

}

void SamplerRenderer::SetProperty( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator )
{
	sampler           = _sampler;
	camera            = _camera;
	surfaceIntegrator = _surfaceIntegrator;
	pAccelerator      = _pAccelerator;
}

void SamplerRenderer::Render( const Scene* scene )
{
	Vector2f Resolution = camera->GetFilm()->GetResolution();
	
	int Width = static_cast< int >( Resolution.x );
	int Height = static_cast< int >( Resolution.y );

	Spectrum L;

	clock_t start = clock();

	#ifndef _DEBUG
	#pragma omp parallel for schedule(dynamic , 1) private(L)
	#endif

	for( int iRow = 0; iRow < Height; iRow++ )
	{
		fprintf( stdout , "\rRendering: %1.0fspp %8.2f%%" , ( double )spp , ( double )iRow / ( double )( Height - 1 ) *100 );

		for( int iCol = 0; iCol < Width; iCol++ )
		{
			L = Spectrum( 0 );

			for( int i = 0; i < spp; i++ )
			{
				CameraSample SamplePoint = sampler->GetSamplePoint();

				Ray ray = camera->GenerateRay( iCol , iRow , SamplePoint );

				IntersectRecord record;

				L += Li( scene , &ray , &record );
			}
			
			L /= (double)spp;

			camera->GetFilm()->SetColor( iRow , iCol , L );
		}
	}

	clock_t end = clock();
	double t = ( double )( end - start ) / CLOCKS_PER_SEC;
	printf( "\nRender time: %fs.\n" , t );
	Log::Get().Info( "\nRender time: %fs.\n" , t );

	camera->GetFilm()->Display();
}

Spectrum SamplerRenderer::Li( const Scene* scene , Ray* ray , IntersectRecord* record /* = nullptr  */ ) const
{
	if( pAccelerator->Intersect( *ray , scene , record ) )
	{
		return surfaceIntegrator->Li( scene , this , record , ray , pAccelerator );
	}

	return Spectrum(0.0f);
}

void SamplerRenderer::Deserialization( tinyxml2::XMLElement* RendererRootElement )
{
	RendererRootElement->FirstChildElement( "spp" )->QueryIntText( &spp );
}