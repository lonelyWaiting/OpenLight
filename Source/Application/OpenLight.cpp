#include "Utilities/PCH.h"
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

	// Light
	IMPLEMENT_DYNAMIC_CREATE_DERIVED( AreaLight )

	IMPLEMENT_DYNAMIC_CREATE_DERIVED( PointLight )

	return true;
}

Renderer* DeserializationScene( Scene* scene , Camera*& camera , SurfaceIntegrator* pSurfaceIntegrator , Sampler* pSampler )
{
	FileSystem fs;
	std::wstring SceneFilename = fs.GetSceneFolder() + L"secondScene.xml";

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

int main( void )
{	
	InitRTTI();

	srand( ( unsigned int )time( NULL ) );

	Scene* scene = new Scene;

	Camera* camera = nullptr;

	SurfaceIntegrator* pSurfaceIntegrator = nullptr;

	Sampler* pSampler = nullptr;

	Renderer* renderer = DeserializationScene( scene , camera , pSurfaceIntegrator , pSampler);

	if( renderer != nullptr )
	{
		renderer->Render( scene );
	}

	/*system( "pause" );*/

	return 0;
}