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
	: Renderer()
	, iRow( 0 )
	, iCol( 0 )
{

}

SamplerRenderer::SamplerRenderer( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator , int _spp/*=8*/ )
	: Renderer( _spp )
	, sampler( _sampler )
	, camera( _camera )
	, surfaceIntegrator( _surfaceIntegrator )
	, pAccelerator( _pAccelerator )
	, iRow( 0 )
	, iCol( 0 )
{

}

SamplerRenderer::~SamplerRenderer()
{
	SAFE_DELETE(sampler);
	SAFE_DELETE(camera);
	SAFE_DELETE(surfaceIntegrator);
	SAFE_DELETE(pAccelerator);
}

void SamplerRenderer::SetProperty( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator )
{
	sampler           = _sampler;
	camera            = _camera;
	surfaceIntegrator = _surfaceIntegrator;
	pAccelerator      = _pAccelerator;
}

bool SamplerRenderer::Render( const Scene* scene )
{
	Vector2f Resolution = camera->GetFilm()->GetResolution();
	
	int Width = static_cast< int >( Resolution.x );
	int Height = static_cast< int >( Resolution.y );

	Spectrum L;

	if( iRow >= Height || iCol >= Width )
	{
		return false;
	}
	else if( iRow == 0 && iCol == 0 )
	{
		start = clock();
	}
	int HeightBound = MIN( iRow + 40 , Height );
	int WidthBound = MIN( iCol + 40 , Width );
	
	int ColTileNumber = std::ceil( Width / 40.0 );
	int RowTileNumber = std::ceil( Height / 40.0 );
	int CurrentTileIndex = ( iRow / 40 ) * ColTileNumber + std::ceil( ( iCol + 1 ) / 40.0 );
	float percent = ( float )CurrentTileIndex / ( ColTileNumber * RowTileNumber );

	#ifndef _DEBUG
	#pragma omp parallel for schedule(dynamic , 1) private(L)
	#endif

	for( int Row = iRow; Row < HeightBound; Row++ )
	{
		fprintf( stdout , "\rRendering: %1.0fspp %8.2f%%" , ( float )spp , ( percent + ( Row - iRow ) / ( 40.0 * ColTileNumber * RowTileNumber ) ) * 100 );

		for( int Col = iCol; Col < WidthBound; Col++ )
		{
			L = Spectrum( 0 );

			for( int i = 0; i < spp; i++ )
			{
				CameraSample SamplePoint = sampler->GetSamplePoint();

				Rayf ray = camera->GenerateRay( Col , Row , SamplePoint );

				IntersectRecord record;

				L += Li( scene , &ray , &record );
			}
			
			L /= ( float )spp;

			camera->GetFilm()->SetColor( Row , Col , L );
		}
	}

	if( iCol + 40 < Width )
	{
		iCol = iCol + 40;
	}
	else if( iCol + 40 >= Width )
	{
		iCol = 0;
		iRow = iRow + 40;
	}

	if( iRow >= Height )
	{
		clock_t end = clock();
		float t = ( float )( end - start ) / CLOCKS_PER_SEC;
		printf( "\nRender time: %fs.\n" , t );

		// 输出到文件
		camera->GetFilm()->Display();
	}

	return true;
}

Spectrum SamplerRenderer::Li( const Scene* scene , Rayf* ray , IntersectRecord* record /* = nullptr  */ ) const
{
	if( pAccelerator->Intersect( *ray , scene , record ) )
	{
		return surfaceIntegrator->Li( scene , this , record , ray , pAccelerator );
	}

	return scene->GetEnvironmentPtr()->Evalute( *ray );
}

void SamplerRenderer::Deserialization( tinyxml2::XMLElement* RendererRootElement )
{
	RendererRootElement->QueryIntAttribute( "spp" , &spp );
}

void SamplerRenderer::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		pRootElement->SetAttribute( "spp" , spp );
	}
}

void SamplerRenderer::ResetRender()
{ 
	iRow = iCol = 0;
}

Accelerator* SamplerRenderer::GetAccelerator()
{
	return pAccelerator;
}