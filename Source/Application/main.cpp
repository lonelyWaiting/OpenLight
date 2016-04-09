#include "PCH.h"
#include "Core/Scene.h"
#include "Renderer/SamplerRenderer.h"
#include "Primitive/Primitive.h"
#include "Math/Transform.h"
#include "Math/Point3.h"
#include "Shapes/Sphere.h"
#include "Camera/ThinLensCamera.h"
#include "Sampler/PureRandomSampler.h"
#include "Spectrum/Spectrum.h"
#include "Utilities/srString.h"
#include "IO/FileSystem.h"
#include "tinyxml2.h"

using namespace tinyxml2;

void PaserScene( Scene* scene , Camera*& camera )
{
	FileSystem fs;
	std::wstring SceneFilename = fs.GetSceneFolder() + L"FirstScene.xml";

	XMLDocument doc;
	doc.LoadFile( srString::ToAscii( SceneFilename ).c_str() );

	// ----------------------------------Primitive---------------------------------------------
	XMLElement* PrimitiveElement = doc.FirstChildElement()->FirstChildElement( "primitive" );
	XMLElement* PrimitiveShapeElement = PrimitiveElement->FirstChildElement( "shape" );

	const char* ShapeName = PrimitiveShapeElement->FirstChildElement( "type" )->GetText();

	XMLElement* PrimitivePosiitonElement = PrimitiveShapeElement->FirstChildElement( "transform" )->FirstChildElement( "position" );
	float PrimitivePosX , PrimitivePosY , PrimitivePosZ;
	PrimitivePosiitonElement->FirstChildElement( "x" )->QueryFloatText( &PrimitivePosX );
	PrimitivePosiitonElement->FirstChildElement( "y" )->QueryFloatText( &PrimitivePosY );
	PrimitivePosiitonElement->FirstChildElement( "z" )->QueryFloatText( &PrimitivePosZ );

	float radius;
	PrimitiveShapeElement->FirstChildElement( "radius" )->QueryFloatText( &radius );

	XMLElement* MaterialElement = PrimitiveElement->FirstChildElement( "material" );
	XMLElement* MaterialSurfaceColorElement = MaterialElement->FirstChildElement( "color" );
	float r , g , b;
	MaterialSurfaceColorElement->FirstChildElement( "r" )->QueryFloatText( &r );
	MaterialSurfaceColorElement->FirstChildElement( "g" )->QueryFloatText( &g );
	MaterialSurfaceColorElement->FirstChildElement( "b" )->QueryFloatText( &b );

	// ---------------------------------Film---------------------------------------------
	XMLElement* FilmElement = doc.FirstChildElement()->FirstChildElement( "Film" );
	char* FilmFilename = new char[20];
	strcpy_s( FilmFilename , 20 , FilmElement->FirstChildElement( "filename" )->GetText() );
	int FilmWidth , FilmHeight;
	FilmElement->FirstChildElement( "width" )->QueryIntText( &FilmWidth );
	FilmElement->FirstChildElement( "height" )->QueryIntText( &FilmHeight );

	// ---------------------------------Camera---------------------------------------------
	XMLElement* CameraElement = doc.FirstChildElement()->FirstChildElement( "Camera" );
	const char* CameraType = CameraElement->FirstChildElement( "type" )->GetText();
	float CameraPosX , CameraPosY , CameraPosZ;
	CameraElement->FirstChildElement( "Position" )->FirstChildElement( "x" )->QueryFloatText( &CameraPosX );
	CameraElement->FirstChildElement( "Position" )->FirstChildElement( "y" )->QueryFloatText( &CameraPosY );
	CameraElement->FirstChildElement( "Position" )->FirstChildElement( "z" )->QueryFloatText( &CameraPosZ );

	float TargetX , TargetY , TargetZ;
	CameraElement->FirstChildElement( "Target" )->FirstChildElement( "x" )->QueryFloatText( &TargetX );
	CameraElement->FirstChildElement( "Target" )->FirstChildElement( "y" )->QueryFloatText( &TargetY );
	CameraElement->FirstChildElement( "Target" )->FirstChildElement( "z" )->QueryFloatText( &TargetZ );

	float LensFocus;
	CameraElement->FirstChildElement( "LensFocus" )->QueryFloatText( &LensFocus );

	float LensRadius;
	CameraElement->FirstChildElement( "LensRadius" )->QueryFloatText( &LensRadius );

	float ApertureWidth , ApertureHeight;
	CameraElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "width" )->QueryFloatText( &ApertureWidth );
	CameraElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "height" )->QueryFloatText( &ApertureHeight );

	float ApertureDistance;
	CameraElement->FirstChildElement( "ApertureDistance" )->QueryFloatText( &ApertureDistance );

	float ExposureTime;
	CameraElement->FirstChildElement( "ExposureTime" )->QueryFloatText( &ExposureTime );

	float ViewDistance;
	CameraElement->FirstChildElement( "ViewDistance" )->QueryFloatText( &ViewDistance );

	// -------------------------Create Scene------------------------------------------
	Primitive* primitive = new Primitive;

	Transform SphereObjectToWorld = Translate( Vector3f( PrimitivePosX , PrimitivePosY , PrimitivePosZ ) );
	Transform SphereWorldToObject = Inverse( SphereObjectToWorld );

	Shape* sphereShape = nullptr;
	if( !std::strcmp( "sphere" , ShapeName ) )
	{
		// 0表示一致
		sphereShape = new Sphere( &SphereObjectToWorld , &SphereWorldToObject , radius );
	}
	else
	{
		Assert( "isn't a sphere , i cant't recognize" );
	}

	primitive->SetShape( sphereShape );
	float RGB[3] = { r , g , b };
	Spectrum diffuseColor;
	diffuseColor = Spectrum::FromRGB( RGB );
	primitive->SetDiffuseColor( diffuseColor );

	scene->AddObject( *primitive );

	// ----------------------Film-------------------------
	Film* film = new Film();		// 使用默认800*600大小，默认名称"result.ppm"
	film->SetFilename( FilmFilename );
	film->SetResolution( FilmWidth , FilmHeight );

	// -------------------Camera---------------------------
	if( !std::strcmp( "ThinLens" , CameraType ) )
	{
		camera = new ThinLensCamera;
		camera->SetCameraPositionAndLookAt( Point3f( CameraPosX , CameraPosY , CameraPosZ ) , Point3f( TargetX , TargetY , TargetZ ) );
		camera->SetApertureResolution( Vector2f( ApertureWidth , ApertureHeight ) );
		camera->SetApertureDistance( ApertureDistance );
		camera->SetExposureTime( ExposureTime );
		camera->SetViewDistance( ViewDistance );
		camera->SetFilm( film );
		( ( ThinLensCamera* )camera )->SetLensFocus( LensFocus );
		( ( ThinLensCamera* )camera )->SetLensRadius( LensRadius );
		camera->UpdateProperty();
	}
	else
	{
		Assert( "Fuck , isn't ThinLens Camera" );
	}
}

int main( void )
{
	
	Scene* scene = new Scene;

	Camera* camera = nullptr;

	PaserScene( scene , camera );

	PureRandomSampler* RandomSampler = new PureRandomSampler;

	SamplerRenderer* renderer = new SamplerRenderer( RandomSampler , camera );

	renderer->Render( scene );

	return 0;
}