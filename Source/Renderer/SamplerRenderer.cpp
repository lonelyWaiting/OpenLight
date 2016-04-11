#include "PCH.h"
#include "Spectrum/Spectrum.h"
#include "Primitive/IntersectRecord.h"
#include "Core/Scene.h"
#include "Math/Vector2.h"
#include "Camera/Camera.h"
#include "Sampler/Sampler.h"
#include "SamplerRenderer.h"

SamplerRenderer::SamplerRenderer( Sampler* _sampler , Camera* _camera )
	: sampler( _sampler )
	, camera( _camera )
{

}

void SamplerRenderer::Render( const Scene* scene )
{
	Vector2f Resolution = camera->GetFilm()->GetResolution();
	
	float Width  = Resolution.x;
	float Height = Resolution.y;

	for( int iRow = 0; iRow < Height; iRow++ )
	{
		for( int iCol = 0; iCol < Width; iCol++ )
		{
			Spectrum L( 0 );

			for( int i = 0; i < 64; i++ )
			{
				CameraSample SamplePoint = sampler->GetSamplePoint();

				Ray ray = camera->GenerateRay( iCol , iRow , SamplePoint );

				L += scene->Trace( ray );
			}
			
			L /= 64;

			camera->GetFilm()->SetColor( iRow , iCol , L );
		}
	}

	camera->GetFilm()->Display();
}

Spectrum SamplerRenderer::Li( const Scene* scene , const Sampler* sample , RandomNumberGenerator& rng , IntersectRecord* record /* = nullptr  */ , Spectrum* T /* = nullptr */ ) const
{
	//Spectrum LocalT;

	//if( T == nullptr )
	//{
	//	T = &LocalT;
	//}

	//IntersectRecord LocalIntersectRecord;

	//if( record == nullptr )
	//{
	//	record = &LocalIntersectRecord;
	//}

	//Spectrum Li = 0.0f;

	//// 检测是否有图元与之相交
	//if( scene->Intersect( ray , record ) )
	//{
	//	Li = surfaceIntegrator->Li( scene , this , ray , *record , sample , rng );
	//}
	//else
	//{
	//	for( uint32_t i = 0; i < scene->Lights.size(); i++ )
	//	{
	//		Li += scene->Lights[i]->Le( ray );
	//	}
	//}

	//return ( ( *T ) * Li );
	
	Spectrum L = 0.0f;

	return ( ( *T ) * L );
}

Spectrum SamplerRenderer::Transmittance( const Scene* scene , const Sampler* sample , RandomNumberGenerator& rng ) const
{
	return Spectrum( 0 );
}