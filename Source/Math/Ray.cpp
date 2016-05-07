#include "Utilities/PCH.h"
#include "Ray.h"

Ray::Ray()
	: MinT( 0.0f )
	, MaxT( Infinity )
	, time( 0.0f )
	, depth( 0 )
{

}

Ray::Ray( const Point3f &Origin , const Vector3f &Direction , double tMin , double tMax /*= Infinity*/ , double time /*= 0.0f*/ , int depth /*= 0 */ )
	: Origin( Origin )
	, Direction( Direction )
	, MinT( tMin )
	, MaxT( tMax )
	, time( time )
	, depth( depth )
{

}

Point3f Ray::operator()( double t ) const
{
	return Origin + Direction * t;
}

Ray::Ray( const Point3f& Origin , const Vector3f& Direction , const Ray& parent , double tmin , double tmax /*= Infinity*/ )
	: Origin( Origin )
	, Direction( Direction )
	, MinT( tmin )
	, MaxT( tmax )
	, time( parent.time )
	, depth( parent.depth + 1 )
{

}

std::ostream& operator << ( std::ostream &os , const Ray &ray )
{
	os << "[Origin = " << ray.Origin << " , Directtion = " << ray.Direction << " , tMin = " << ray.MinT << " , tMax = " << ray.MaxT << " , time = " << ray.time << "]";

	return os;
}

Ray::Ray( const Ray& ray )
{
	Origin = ray.Origin;
	Direction = ray.Direction;
	time = ray.time;
	MinT = ray.MinT;
	MaxT = ray.MaxT;
	depth = ray.depth;
}