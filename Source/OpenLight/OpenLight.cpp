#include <Utilities/PCH.h>
#include "tinyxml2.h"
#include "OpenLight.h"

#include "Core/Scene.h"

void Scene::InitRTTI()
{
	// Shape
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( Sphere )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( Triangle )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( TriangleMesh )

	// Renderer
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( SamplerRenderer )

	// Sampler
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( NRooksSampler )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PureRandomSampler )

	// Camera
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PinholeCamera )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( ThinLensCamera )

	// Material
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( DiffuseMaterial )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( GlassMaterial )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PureReflectionMaterial )

	// Integrator
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( WhittedIntegrator )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( DirectIntegrator )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PathIntegrator )

	// Light
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( AreaLight )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PointLight )

	// Texture
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( CheckboardTexture )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( ConstantTexture )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( ImageTexture )

	// Environment
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( ConstantEnvironment )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( HDREnvironment )
}


void Scene::Deserialization( const char* filename /* = nullptr */ )
{
	SAFE_DELETE(mRendererPtr);

	mFilename = filename;

	tinyxml2::XMLDocument doc;
	doc.LoadFile( filename );

	tinyxml2::XMLElement* pRootElement = doc.FirstChildElement();

	// ---------------------------------------Environment--------------------------------------------
	tinyxml2::XMLElement* pEnvironmentElement = pRootElement->FirstChildElement( "Environment" );
	if( pEnvironmentElement )
	{
		const char* str = pEnvironmentElement->Attribute( "type" );

		Environment* pEnvironment = Environment::Create( pEnvironmentElement->Attribute( "type" ) );
		pEnvironment->Deserialization( pEnvironmentElement );
		AddEnvironment( pEnvironment );
	}
	else
	{
		Environment* pEnvironment = new ConstantEnvironment;
		AddEnvironment( pEnvironment );
	}

	// -------------------------------------Primitive-------------------------------------------------
	tinyxml2::XMLElement* pPrimitiveElement = pRootElement->FirstChildElement( "primitive" );
	while( pPrimitiveElement )
	{
		Primitive* pPrimitive = new Primitive;
		pPrimitive->Deserialization( pPrimitiveElement );
		AddEntity( pPrimitive );
		AddLight( pPrimitive->GetAreaLight() );
		pPrimitiveElement = pPrimitiveElement->NextSiblingElement( "primitive" );
	}

	// ---------------------------------------Light--------------------------------------------------
	tinyxml2::XMLElement* pLightElement = pRootElement->FirstChildElement( "light" );
	while( pLightElement )
	{
		const char* LightType = pLightElement->Attribute( "type" );

		Light* light = Light::Create( LightType );
		light->Deserialization( pLightElement );
		AddLight( light );

		pLightElement = pLightElement->NextSiblingElement( "light" );
	}

	// --------------------------------------Film-----------------------------------------------------
	Film* film = new Film;
	tinyxml2::XMLElement* pFilmElement = pRootElement->FirstChildElement( "Film" );
	film->Deserialization( pFilmElement );

	// --------------------------------------Camera--------------------------------------------------
	tinyxml2::XMLElement* pCameraElement = pRootElement->FirstChildElement( "Camera" );
	const char* CameraType = pCameraElement->Attribute( "type" );

	mCameraPtr = Camera::Create( CameraType );
	mCameraPtr->SetFilm( film );
	mCameraPtr->Deserialization( pCameraElement );

	// --------------------------------------Integrator---------------------------------------------
	tinyxml2::XMLElement* pIntegratorRootElement = pRootElement->FirstChildElement( "Integrator" );
	const char* IntegratorType = pIntegratorRootElement->Attribute( "type" );

	mSurfaceIntegratorPtr = SurfaceIntegrator::Create( IntegratorType );
	mSurfaceIntegratorPtr->Deserialization( pIntegratorRootElement );

	// --------------------------------------Sampler--------------------------------------------------
	tinyxml2::XMLElement* pSamplerRootElement = pRootElement->FirstChildElement( "Sampler" );
	const char* SamplerType = pSamplerRootElement->Attribute( "type" );

	mSamplerPtr = Sampler::Create( SamplerType );
	mSamplerPtr->Deserialization( pSamplerRootElement );

	// --------------------------------------Renderer---------------------------------------------------
	tinyxml2::XMLElement* pRendererRootElement = pRootElement->FirstChildElement( "Renderer" );
	const char* RendererType = pRendererRootElement->Attribute( "type" );

	mRendererPtr = Renderer::Create( RendererType );
	mRendererPtr->Deserialization( pRendererRootElement );

	// -------------------------------------Build Acceleration Structure---------------------------------
	Grid* pGrid = new Grid;
	pGrid->Setup( this );

	mRendererPtr->SetProperty( mSamplerPtr , mCameraPtr , mSurfaceIntegratorPtr , pGrid );
}

void Scene::Serialization( const char* filename /*= nullptr*/ )
{
	if (filename == nullptr || strcmp(filename, "") == 0)	return;

	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLElement* pRoot = xmlDoc.NewElement( "Root" );

	// Attach it to the XMLDocument
	xmlDoc.InsertFirstChild( pRoot );

	// -----------------------------------------Scene-------------------------------------------------
	{
		tinyxml2::XMLElement* pEnvironmentElement = xmlDoc.NewElement( "Environment" );

		mEnvironmentPtr->Serialization( xmlDoc , pEnvironmentElement );

		pRoot->InsertEndChild( pEnvironmentElement );
	}

	{
		for( auto& entity : mEntityList )
		{
			tinyxml2::XMLElement* pEntityElement = xmlDoc.NewElement( "primitive" );

			pRoot->InsertEndChild( pEntityElement );

			entity->Serialization( xmlDoc , pEntityElement );
		}
	}

	{
		for( auto& light : mLightList )
		{
			if( !strcmp( light->GetName() , "AreaLight" ) )
			{
				// AreaLight just attach to primitive node
				// AreaLight will serializ through primitive's serialization function
				continue;
			}

			tinyxml2::XMLElement* pLightElement = xmlDoc.NewElement( "Light" );

			pRoot->InsertEndChild( pLightElement );

			light->Serialization( xmlDoc , pLightElement );
		}
	}

	{
		tinyxml2::XMLElement* pCameraElement = xmlDoc.NewElement( "Camera" );

		pRoot->InsertEndChild( pCameraElement );

		mCameraPtr->Serialization( xmlDoc , pCameraElement );
	}

	{
		tinyxml2::XMLElement* pIntegratorElement = xmlDoc.NewElement( "Integrator" );

		pRoot->InsertEndChild( pIntegratorElement );

		mSurfaceIntegratorPtr->Deserialization( pIntegratorElement );
	}


	{
		tinyxml2::XMLElement* pSamplerElement = xmlDoc.NewElement( "Sampler" );

		pRoot->InsertEndChild( pSamplerElement );

		mSamplerPtr->Serialization( xmlDoc , pSamplerElement );
	}

	{
		tinyxml2::XMLElement* pRendererElement = xmlDoc.NewElement( "Renderer" );

		pRoot->InsertEndChild( pRendererElement );

		mRendererPtr->Serialization( xmlDoc , pRendererElement );
	}

	// save to disk
	xmlDoc.SaveFile( filename );
}

namespace OpenLight
{
	Scene scene;
};