#include "Utilities/PCH.h"
#include "Transform.h"
#include "Bound3.h"
#include "Matrix4.h"
#include "Point3.h"
#include "Ray.h"
#include "Vector3.h"

Transform::Transform()
	:matrix()
	,matrix_inv()
{

}

Transform::Transform( const float m[4][4] )
{
	matrix = Matrix4f( m[0][0] , m[0][1] , m[0][2] , m[0][3] ,
					   m[1][0] , m[1][1] , m[1][2] , m[1][3] ,
					   m[2][0] , m[2][1] , m[2][2] , m[2][3] ,
					   m[3][0] , m[3][1] , m[3][2] , m[3][3] );

	matrix_inv = Inverse( matrix );
}

Transform::Transform( const Matrix4f& m )
	: matrix( m )
	, matrix_inv( Inverse( m ) )
{

}

Transform::Transform( const Matrix4f& m , const Matrix4f& invm )
	: matrix( m )
	, matrix_inv( invm )
{

}

Transform& Transform::operator = ( const Transform& rhs )
{
	matrix = rhs.matrix;

	matrix_inv = rhs.matrix_inv;

	return ( *this );
}

Transform Inverse( const Transform& t )
{
	return Transform( t.matrix_inv , t.matrix );
}

Transform Transpose( const Transform& t )
{
	return Transform( Transpose( t.matrix ) , Transpose( t.matrix_inv ) );
}

bool Transform::operator == ( const Transform& t ) const
{
	return t.matrix == matrix && t.matrix_inv == matrix_inv;
}

bool Transform::operator != ( const Transform& t ) const
{
	return t.matrix != matrix || t.matrix_inv != matrix_inv;
}

const Matrix4f& Transform::GetMatrix() const
{
	return matrix;
}

const Matrix4f& Transform::GetInvMatrix() const
{
	return matrix_inv;
}

Point3f Transform::operator() ( const Point3f& p ) const
{
	return p * matrix;
}

Vector3f Transform::operator() ( const Vector3f& v ) const
{
	return v * matrix;
}

Bound3f Transform::operator() ( const Bound3f& bbox ) const
{
	const Transform& M = *this;

	Bound3f box( M( Point3f( bbox.pMin.x , bbox.pMin.y , bbox.pMin.z ) ) );
	box.ExpendToInclude( M( Point3f( bbox.pMin.x , bbox.pMin.y , bbox.pMax.z ) ) );
	box.ExpendToInclude( M( Point3f( bbox.pMin.x , bbox.pMax.y , bbox.pMin.z ) ) );
	box.ExpendToInclude( M( Point3f( bbox.pMin.x , bbox.pMax.y , bbox.pMax.z ) ) );

	box.ExpendToInclude( M( Point3f( bbox.pMax.x , bbox.pMin.y , bbox.pMin.z ) ) );
	box.ExpendToInclude( M( Point3f( bbox.pMax.x , bbox.pMin.y , bbox.pMax.z ) ) );
	box.ExpendToInclude( M( Point3f( bbox.pMax.x , bbox.pMax.y , bbox.pMin.z ) ) );
	box.ExpendToInclude( M( Point3f( bbox.pMax.x , bbox.pMax.y , bbox.pMax.z ) ) );

	return box;
}

Rayf Transform::operator() ( const Rayf& r ) const
{
	Point3f origin = ( *this )( r.Origin );

	Vector3f dir = ( *this )( r.Direction );

	return Rayf( origin , dir , r.MinT , r.MaxT , r.time , r.depth );
}

Transform Transform::operator* ( const Transform& t2 ) const
{
	// 注意inv的相乘顺序
	return Transform( matrix * t2.matrix , t2.matrix_inv * matrix_inv );
}

Transform Perspective( float fov , float aspect , float zNear , float zFar )
{
	float t = 1.0f / tan( ToRadians( fov * 0.5f ) );
	float temp = zFar / ( zFar - zNear );

	Matrix4f perspective( 1 / aspect * t , 0.0f , 0.0f , 0.0f ,
						  0.0f , t , 0.0f , 0.0f ,
						  0.0f , 0.0f , temp , 1.0f ,
						  0.0f , 0.0f , -1 * zNear * temp , 0.0f );

	return Transform( perspective , Inverse( perspective ) );
}

Transform Orthographic( float zNear , float zFar )
{
	// (z - zNear) / (zFar - zNear)
	// when z = zNear , the above formula result is 0
	// when z < zNear , the above formula result is less than 0
	// when z = zFar  , the above formula result is 1
	// when z > zFar  , the above formula result is more than 1
	return Translate( Vector3f( 0.0f , 0.0f , -zNear ) ) * Scale( 1 , 1 , 1 / ( zFar - zNear ) );
}

Transform LookAt( const Point3f& eye , const Point3f& look , const Vector3f& up )
{
	Matrix4f WorldToCamera;

	Vector3f zAxis = Normalize( look - eye );

	if( Cross( Normalize( up ) , zAxis ).Length() == 0 )
	{
		return Transform();
	}

	Vector3f xAxis = Normalize( Cross( Normalize( up ) , zAxis ) );

	Vector3f yAxis = Cross( zAxis , xAxis );

	WorldToCamera.m00 = xAxis.x; WorldToCamera.m01 = yAxis.x; WorldToCamera.m02 = zAxis.x; WorldToCamera.m03 = 0.0f;
	WorldToCamera.m10 = xAxis.y; WorldToCamera.m11 = yAxis.y; WorldToCamera.m12 = zAxis.y; WorldToCamera.m13 = 0.0f;
	WorldToCamera.m20 = xAxis.z; WorldToCamera.m21 = yAxis.z; WorldToCamera.m22 = zAxis.z; WorldToCamera.m23 = 0.0f;

	WorldToCamera.m30 = -( Dot( xAxis , Vector3f( eye ) ) );

	WorldToCamera.m31 = -( Dot( yAxis , Vector3f( eye ) ) );

	WorldToCamera.m32 = -( Dot( zAxis , Vector3f( eye ) ) );

	WorldToCamera.m33 = 1.0f;


	Matrix4f CameraToWorld;
	CameraToWorld.m00 = xAxis.x;	CameraToWorld.m01 = xAxis.y;	CameraToWorld.m02 = xAxis.z;	CameraToWorld.m03 = 0.0f;
	CameraToWorld.m10 = yAxis.x;	CameraToWorld.m11 = yAxis.y;	CameraToWorld.m12 = yAxis.z;	CameraToWorld.m13 = 0.0f;
	CameraToWorld.m20 = zAxis.x;	CameraToWorld.m21 = zAxis.y;	CameraToWorld.m22 = zAxis.z;	CameraToWorld.m23 = 0.0f;
	CameraToWorld.m30 = eye.x;		CameraToWorld.m31 = eye.y;		CameraToWorld.m32 = eye.z;		CameraToWorld.m33 = 1.0f;

	return Transform( WorldToCamera , CameraToWorld );
}

Transform Rotate( const Vector3f& axis , float theta )
{
	Vector3f a = Normalize( axis );

	float x = a.x;
	float y = a.y;
	float z = a.z;

	float cosTheta = std::cos( ToRadians( theta ) );
	float OneSubCos = 1.0f - cosTheta;
	float sinTheta = std::sin( ToRadians( theta ) );

	Matrix4f m( x * x * OneSubCos + cosTheta , x * y * OneSubCos + z * sinTheta , x * z * OneSubCos - y * sinTheta , 0.0f ,
				x * y * OneSubCos - z * sinTheta , y * y * OneSubCos + cosTheta , y * z * OneSubCos + x * sinTheta , 0.0f ,
				x * z * OneSubCos + y * sinTheta , y * z * OneSubCos - x * sinTheta , z * z * OneSubCos + cosTheta , 0.0f ,
				0.0f , 0.0f , 0.0f , 1.0f );

	return Transform( m , Transpose( m ) );
}

Transform RotateZ( float theta )
{
	float fSin = sin( ToRadians( theta ) );
	float fCos = cos( ToRadians( theta ) );

	return Transform( Matrix4f( fCos , -fSin , 0.0f , 0.0f ,
					  fSin , fCos , 0.0f , 0.0f ,
					  0.0f , 0.0f , 1.0f , 0.0f ,
					  0.0f , 0.0f , 0.0f , 1.0f ) ,
					  Matrix4f( fCos , fSin , 0.0f , 0.0f ,
					  -fSin , fCos , 0.0f , 0.0f ,
					  0.0f , 0.0f , 1.0f , 0.0f ,
					  0.0f , 0.0f , 0.0f , 1.0f ) );
}

Transform RotateY( float theta )
{
	float fSin = sin( ToRadians( theta ) );
	float fCos = cos( ToRadians( theta ) );

	return Transform( Matrix4f( fCos , 0.0f , fSin , 0.0f ,
					  0.0f , 1.0f , 0.0f , 0.0f ,
					  -fSin , 0.0f , fCos , 0.0f ,
					  0.0f , 0.0f , 0.0f , 1.0f ) ,
					  Matrix4f( fCos , 0.0f , -fSin , 0.0f ,
					  0.0f , 1.0f , 0.0f , 0.0f ,
					  fSin , 0.0f , fCos , 0.0f ,
					  0.0f , 0.0f , 0.0f , 1.0f ) );
}

Transform RotateX( float theta )
{
	float fSin = sin( ToRadians( theta ) );
	float fCos = cos( ToRadians( theta ) );

	return Transform( Matrix4f( 1.0f , 0.0f , 0.0f , 0.0f ,
					  0.0f , fCos , -fSin , 0.0f ,
					  0.0f , fSin , fCos , 0.0f ,
					  0.0f , 0.0f , 0.0f , 1.0f ) ,
					  Matrix4f( 1.0f , 0.0f , 0.0f , 0.0f ,
					  0.0f , fCos , fSin , 0.0f ,
					  0.0f , -fSin , fCos , 0.0f ,
					  0.0f , 0.0f , 0.0f , 1.0f ) );
}

Transform Scale( float x , float y , float z )
{
	return Transform( Matrix4f( x , 0.0f , 0.0f , 0.0f , 0.0f , y , 0.0f , 0.0f , 0.0f , 0.0f , z , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f ) ,
					  Matrix4f( 1.0f / x , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f / y , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f / z , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f ) );
}

Transform Translate( const Vector3f& delta )
{
	return Transform( Matrix4f( 1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f , 0.0f , delta.x , delta.y , delta.z , 1.0f ) ,
					  Matrix4f( 1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f , 0.0f , -delta.x , -delta.y , -delta.z , 1.0f ) );
}

/*
	A[0][0]	A[0][1]		x				B[0]
								=	
	A[1][0]	A[1][1]		y				B[1]

	计算x , y
*/
bool SolveLinearSystem2x2( const float A[2][2] , const float B[2] , float* x , float* y )
{
	float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];

	if( fabs( det ) < 1e-10f )
	{
		return false;
	}

	*x = ( A[1][1] * B[0] - A[0][1] * B[1] ) / det;
	*y = ( A[0][0] * B[1] - A[1][0] * B[0] ) / det;

	if( std::isnan( *x ) || std::isnan( *y ) )
	{
		return false;
	}

	return true;
}