#include "PCH.h"
#include "Core/Scene.h"
#include "Renderer/SamplerRenderer.h"
#include "Primitive/Primitive.h"
#include "Math/Transform.h"
#include "Math/Point3.h"
#include "Shapes/Sphere.h"
#include "Camera/ThinLensCamera.h"
#include "Camera/PinholeCamera.h"
#include "Sampler/PureRandomSampler.h"
#include "Spectrum/Spectrum.h"
#include "Utilities/srString.h"
#include "IO/FileSystem.h"
#include "Sampler/NRooksSampler.h"
#include "Integrator/WhittedIntegrator.h"
#include "BRDF/Lambertian.h"
#include "Light/PointLight.h"
#include "tinyxml2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace tinyxml2;

void ParseScene( Scene* scene , Camera*& camera )
{
	FileSystem fs;
	std::wstring SceneFilename = fs.GetSceneFolder() + L"FirstScene.xml";

	XMLDocument doc;
	doc.LoadFile( srString::ToAscii( SceneFilename ).c_str() );

	// ----------------------------------Primitive---------------------------------------------
	Primitive* primitive = new Primitive;
	XMLElement* PrimitiveElement = doc.FirstChildElement()->FirstChildElement( "primitive" );
	primitive->ParsePrimitive( PrimitiveElement );

	Lambertian* lambertian = new Lambertian( Spectrum( 0.7 ) );
	primitive->
	scene->AddObject( *primitive );

	PointLight* pLight = new PointLight( Translate( Vector3f( 10 , 2 , 5 ) ) , Spectrum( 0.5f ) );
	scene->AddLight( pLight );
	// ---------------------------------Film---------------------------------------------
	Film* film = new Film();
	XMLElement* FilmElement = doc.FirstChildElement()->FirstChildElement( "Film" );
	film->ParseFilm( FilmElement );

	// ---------------------------------Camera---------------------------------------------
	XMLElement* CameraElement = doc.FirstChildElement()->FirstChildElement( "Camera" );
	const char* CameraType = CameraElement->FirstChildElement( "type" )->GetText();
	if( !std::strcmp( "ThinLens" , CameraType ) )
	{
		SAFE_DELETE( camera );
		
		camera = new ThinLensCamera;

		camera->SetFilm( film );

		camera->ParseCamera( CameraElement );	
	}
	else if( !std::strcmp( "Pinhole" , CameraType ) )
	{
		SAFE_DELETE( camera );

		camera = new PinholeCamera;

		camera->SetFilm( film );

		camera->ParseCamera( CameraElement );
	}
	else
	{
		Assert( "don't support \'%s\' Camera" , CameraType );
	}
}

int main( void )
{	
	Scene* scene = new Scene;

	Camera* camera = nullptr;

	ParseScene( scene , camera );

	/*PureRandomSampler* RandomSampler = new PureRandomSampler;*/
	
	NRooksSampler* nRooksSampler = new NRooksSampler;

	WhittedIntegrator* integrator = new WhittedIntegrator;
	integrator->SetMaxRecusiveDepth( 5 );
	SamplerRenderer* renderer = new SamplerRenderer( nRooksSampler , camera , integrator );

	renderer->Render( scene );

	return 0;
}