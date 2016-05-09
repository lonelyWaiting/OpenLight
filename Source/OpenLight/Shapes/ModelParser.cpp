#include <Utilities/PCH.h>
#include "Math/Point3.h"
#include "Math/Point2.h"
#include "Math/Normal.h"
#include "Triangle.h"
#include "IO/FileSystem.h"
#include "IO/Log.h"
#include <Utilities/srString.h>
#include "Shapes/Triangle.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelParser.h"

void ModelParse( const std::string& filename , Point3f*& points , Normal*& normals , Triangle*& triangles , int& VertexCount , int& FaceCount )
{
	FileSystem fs;
	std::wstring name = fs.GetModelFolder() + srString::ToUnicode( filename );

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( srString::ToAscii( name ) , aiProcess_Triangulate );

	if( !scene )
	{
		Assert( "can't read model file context" );
	}

	if( scene->HasMeshes() )
	{
		VertexCount = 0;
		FaceCount = 0;

		for( unsigned int i = 0; i < scene->mNumMeshes; i++ )
		{
			aiMesh* mesh = scene->mMeshes[i];

			VertexCount += mesh->mNumVertices;
			FaceCount   += mesh->mNumFaces;
		}

		SAFE_DELETE( points );
		points = new Point3f[VertexCount];

		SAFE_DELETE( normals );
		normals = new Normal[VertexCount];

		SAFE_DELETE( triangles );
		triangles = new Triangle[FaceCount];

		int Vertexindex = 0;
		int FaceIndex = 0;
		for( unsigned int i = 0; i < scene->mNumMeshes; i++ )
		{
			aiMesh* mesh = scene->mMeshes[i];

			int count = mesh->mNumVertices;

			for( int j = 0; j < count; j++ , Vertexindex++ )
			{
				points[Vertexindex]  = Point3f( mesh->mVertices[j].x , mesh->mVertices[j].y , mesh->mVertices[j].z );

				normals[Vertexindex] = Normal( mesh->mNormals[j].x , mesh->mNormals[j].y , mesh->mNormals[j].z );
			}

			for( unsigned int j = 0; j < mesh->mNumFaces; j++ , FaceIndex++ )
			{
				const aiFace& face = mesh->mFaces[j];

				triangles[FaceIndex].Set( face.mIndices[0] , face.mIndices[1] , face.mIndices[2] );
			}
		}
	}
}