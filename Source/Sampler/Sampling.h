#pragma once

#include "Math/Vector3.h"
#include "Math/Point2.h"
#include "Math/Point3.h"

Vector3f UniformSampleHemisphere( const Point2f& u );

Point2f ConcentricSampleDisk( const Point2f& u );

Vector3f CosineSampleHemisphere( const Point2f& u );

Vector3f DensitySampleHemisphere( const Point2f& u , float e );

Vector3f UniformSampleSphere( float u1 , float u2 );

Vector3f UniformSampleCone( float u1 , float u2 , float cosThetaMax , const Vector3f& x , const Vector3f& y , const Vector3f& z );

// 每个方向对应的实体角
float UniformConePDF( float cosThetaMax );

float UniformSpherePDF();

float UniformHemispherePDF();

Point2f MapUnitSquareToUnitDisk( const Point2f& UnitSquareSamplePoint );

Point3f MapUnitSquareToHemisphere( const Point2f& UnitSquareSamplePoint , float exp = 0 );

Point3f MapUnitSquareToSphere( const Point2f& UnitSquareSamplePoint );