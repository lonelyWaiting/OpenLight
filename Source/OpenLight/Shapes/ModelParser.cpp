#include <Utilities/PCH.h>
#include "Math/Point3.h"
#include "Math/Point2.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Triangle.h"
#include "IO/FileSystem.h"
#include "IO/Log.h"
#include <Utilities/srString.h>
#include "Shapes/Triangle.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelParser.h"

void ModelParse( const std::string& filename , Point3f*& points , Vector3f*& normals , Vector2f*& uvs , Triangle*& triangles , int& VertexCount , int& FaceCount )
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
		normals = new Vector3f[VertexCount];

		SAFE_DELETE( uvs );
		/*if (scene->HasTextures())
		{
			uvs = new Vector2f[VertexCount];
		}*/
		uvs = new Vector2f[VertexCount];

		SAFE_DELETE( triangles );
		triangles = new Triangle[FaceCount];

		int Vertexindex = 0;
		int FaceIndex = 0;
		for( unsigned int i = 0; i < scene->mNumMeshes; i++ )
		{
			aiMesh* mesh = scene->mMeshes[i];

			int count = mesh->mNumVertices;

			if (!mesh->mTextureCoords[0])
			{
				SAFE_DELETE_ARRAY(uvs);
				uvs = nullptr;
			}

			for( int j = 0; j < count; j++ , Vertexindex++ )
			{
				points[Vertexindex]  = Point3f( mesh->mVertices[j].x , mesh->mVertices[j].y , mesh->mVertices[j].z );

				normals[Vertexindex] = Vector3f( mesh->mNormals[j].x , mesh->mNormals[j].y , mesh->mNormals[j].z );

				if (uvs)
				{
					uvs[Vertexindex] = Vector2f(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
				}
			}

			for( unsigned int j = 0; j < mesh->mNumFaces; j++ , FaceIndex++ )
			{
				const aiFace& face = mesh->mFaces[j];

				triangles[FaceIndex].Set( face.mIndices[0] , face.mIndices[1] , face.mIndices[2] );
			}
		}
	}
}