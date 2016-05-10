#include <Utilities/PCH.h>
#include "tinyxml2.h"
#include "OpenLight.h"

bool InitRTTI()
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

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PathTracing )

	// Light
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( AreaLight )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PointLight )

	return true;
}

Renderer* DeserializationScene( Scene* scene , Camera*& camera , SurfaceIntegrator*& pSurfaceIntegrator , Sampler*& pSampler )
{
	FileSystem fs;
	std::wstring SceneFilename = fs.GetSceneFolder() + L"Scene.xml";

	tinyxml2::XMLDocument doc;
	doc.LoadFile( srString::ToAscii( SceneFilename ).c_str() );

	// ----------------------------------Primitive---------------------------------------------
	tinyxml2::XMLElement* PrimitiveElement = doc.FirstChildElement()->FirstChildElement( "primitive" );
	while( PrimitiveElement )
	{
		Primitive* primitive = new Primitive;
		Assert( primitive != nullptr );
		primitive->Deserialization( PrimitiveElement );
		scene->AddObject( *primitive );
		scene->AddLight( primitive->GetAreaLight() );
		PrimitiveElement = PrimitiveElement->NextSiblingElement( "primitive" );
	}
	
	// -----------------------------------Light---------------------------------------------
	tinyxml2::XMLElement* LightRootElement = doc.FirstChildElement()->FirstChildElement( "light" );
	while( LightRootElement )
	{
		const char* LightType = LightRootElement->FirstAttribute()->Value();

		Light* light = Light::Create( LightType );
		Assert( light != nullptr );
		light->Deserialization( LightRootElement );
		scene->AddLight( light );

		LightRootElement = LightRootElement->NextSiblingElement( "light" );
	}

	// ---------------------------------Film---------------------------------------------
	Film* film = new Film();
	Assert( film != nullptr );
	tinyxml2::XMLElement* FilmElement = doc.FirstChildElement()->FirstChildElement( "Film" );
	film->Deserialization( FilmElement );

	// ---------------------------------Camera---------------------------------------------
	tinyxml2::XMLElement* CameraElement = doc.FirstChildElement()->FirstChildElement( "Camera" );
	const char* CameraType = CameraElement->FirstAttribute()->Value();
	SAFE_DELETE( camera );

	camera = Camera::Create( CameraType );
	Assert( camera != nullptr );
	camera->SetFilm( film );
	camera->Deserialization( CameraElement );

	// -----------------------------Integrator--------------------------------------
	tinyxml2::XMLElement* IntegratorRootElement = doc.FirstChildElement()->FirstChildElement( "Integrator" );
	const char* IntegratorType = IntegratorRootElement->FirstAttribute()->Value();

	pSurfaceIntegrator = SurfaceIntegrator::Create( IntegratorType );
	Assert( pSurfaceIntegrator != nullptr );
	pSurfaceIntegrator->Deserialization( IntegratorRootElement );

	// --------------------------Sampler--------------------------------------------
	tinyxml2::XMLElement* SamplerRootElement = doc.FirstChildElement()->FirstChildElement( "Sampler" );
	const char* SamplerType = SamplerRootElement->FirstAttribute()->Value();

	pSampler = Sampler::Create( SamplerType );
	Assert( pSampler != nullptr );
	pSampler->Deserialization( SamplerRootElement );
	
	// -------------------------Build Accelerator Structure-----------------------------
	Grid* pGrid = new Grid;
	pGrid->Setup( scene );

	// ----------------------Renderer----------------------------------------
	tinyxml2::XMLElement* RendererRootElement = doc.FirstChildElement()->FirstChildElement( "Renderer" );
	const char* RendererType = RendererRootElement->FirstAttribute()->Value();

	Renderer* renderer = Renderer::Create( RendererType );
	Assert( renderer != nullptr );
	renderer->SetProperty( pSampler , camera , pSurfaceIntegrator , pGrid );
	renderer->Deserialization( RendererRootElement );
	return renderer;
}

bool SerializationScene( Scene* scene , Camera* camera , SurfaceIntegrator* pSurfaceIntegrator , Sampler* pSampler , Renderer* pRenderer )
{
	tinyxml2::XMLDocument xmlDoc;

	// Create Root Node
	tinyxml2::XMLElement* pRoot = xmlDoc.NewElement( "Root" );

	// Attach it to the XMLDocument
	xmlDoc.InsertFirstChild( pRoot );

	// -----------------------------------Serialization Scene-------------------------------------------
	scene->Serialization( xmlDoc , pRoot );

	// ---------------------------------Serialization Camera-----------------------------------------------
	{
		tinyxml2::XMLElement* pElement = xmlDoc.NewElement( "Camera" );
		pRoot->InsertEndChild( pElement );
		camera->Serialization( xmlDoc , pElement );
	}

	// ---------------------------------Serialization Integrator-------------------------------------------
	{
		tinyxml2::XMLElement* pElement = xmlDoc.NewElement( "Integrator" );
		pRoot->InsertEndChild( pElement );
		pSurfaceIntegrator->Serialization( xmlDoc , pElement );
	}

	// ---------------------------------Serialization Sampler--------------------------------------
	{
		tinyxml2::XMLElement* pElement = xmlDoc.NewElement( "Sampler" );
		pRoot->InsertEndChild( pElement );
		pSampler->Serialization( xmlDoc , pElement );
	}

	// -------------------------------Serialzation Renderer------------------------------------------
	{
		tinyxml2::XMLElement* pElement = xmlDoc.NewElement( "Renderer" );
		pRoot->InsertEndChild( pElement );
		pRenderer->Serialization( xmlDoc , pElement );
	}

	FileSystem fs;
	std::wstring filename = fs.GetSceneFolder() + L"Scene.xml";
	tinyxml2::XMLError eResult = xmlDoc.SaveFile( srString::ToAscii( filename ).c_str() );

	return true;
}

//int main( void )
//{	
//	InitRTTI();
//
//	srand( ( unsigned int )time( NULL ) );
//
//	Scene* scene = new Scene;
//
//	Camera* camera = nullptr;
//
//	SurfaceIntegrator* pSurfaceIntegrator = nullptr;
//
//	Sampler* pSampler = nullptr;
//
//	Renderer* renderer = DeserializationScene( scene , camera , pSurfaceIntegrator , pSampler);
//
//	if( renderer != nullptr )
//	{
//		renderer->Render( scene );
//	}
//
//	/*system( "pause" );*/
//
//	SerializationScene( scene , camera , pSurfaceIntegrator , pSampler , renderer );
//
//	return 0;
//}