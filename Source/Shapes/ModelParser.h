#pragma once

#include <string>

class Point3f;
class Normal;
class Point2f;
class Triangle;

void ModelParser( const std::string& filename , Point3f*& points , Normal*& normals , Triangle*& traingles , int& VertexCount , int& FaceCount );