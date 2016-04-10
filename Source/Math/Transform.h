#pragma once

class Ray;
class Vector3f;
class Bound3f;
class Point3f;
class Normal;

#include "Matrix4.h"

class Transform
{
public:
	Transform();

	Transform( const float m[4][4] );

	Transform( const Matrix4f& m );

	Transform( const Matrix4f& m , const Matrix4f& invm );

	Transform& operator = ( const Transform& rhs );

	friend Transform Inverse( const Transform& t );

	friend Transform Transpose( const Transform& t );

	bool operator == ( const Transform& t ) const;

	bool operator != ( const Transform& t ) const;

	const Matrix4f& GetMatrix() const;

	const Matrix4f& GetInvMatrix() const;

	Point3f operator() ( const Point3f& p ) const;

	Vector3f operator() ( const Vector3f& v ) const;

	Ray operator() ( const Ray& r ) const;

	Normal operator() ( const Normal& n ) const;

	Bound3f operator()( const Bound3f& bbox ) const;

	Transform operator* ( const Transform& t2 ) const;

public:
	friend bool SolveLinearSystem2x2( const float A[2][2] , const float B[2] , float* x , float* y );

	friend Transform Translate( const Vector3f& delta );

	friend Transform Scale( float x , float y , float z );

	friend Transform RotateX( float theta );

	friend Transform RotateY( float theta );

	friend Transform RotateZ( float theta );

	friend Transform Rotate( const Vector3f& axis , float theta );

	friend Transform LookAt( const Point3f& eye , const Point3f& look , const Vector3f& up );

	friend Transform Orthographic( float zNear , float zFar );

	friend Transform Perspective( float fov , float aspect , float zNear , float zFar );

public:
	Matrix4f matrix;
	Matrix4f matrix_inv;
};