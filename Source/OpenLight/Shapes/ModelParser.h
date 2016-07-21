#pragma once

#include <string>

class Triangle;

void ModelParse( const std::string& filename , Point3f*& points , Vector3f*& normals , Vector2f*& uvs , Triangle*& traingles , int& VertexCount , int& FaceCount );