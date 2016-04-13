#pragma once

#include "Math/Vector3.h"
#include "Math/Point2.h"
#include "Math/Point3.h"

Vector3f UniformSampleHemisphere( const Point2f& u );

Point2f ConcentricSampleDisk( const Point2f& u );

Vector3f CosineSampleHemisphere( const Point2f& u );

Vector3f DensitySampleHemisphere( const Point2f& u , double e );

Vector3f UniformSampleSphere( double u1 , double u2 );

Vector3f UniformSampleCone( double u1 , double u2 , double cosThetaMax , const Vector3f& x , const Vector3f& y , const Vector3f& z );

// 每个方向对应的实体角
double UniformConePDF( double cosThetaMax );

double UniformSpherePDF();

double UniformHemispherePDF();

Point2f MapUnitSquareToUnitDisk( const Point2f& UnitSquareSamplePoint );

Point3f MapUnitSquareToHemisphere( const Point2f& UnitSquareSamplePoint , double exp = 0 );

Point3f MapUnitSquareToSphere( const Point2f& UnitSquareSamplePoint );