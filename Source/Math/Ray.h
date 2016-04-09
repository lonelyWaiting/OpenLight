#ifndef RAY_H
#define RAY_H

#include "Point3.h"
#include "Vector3.h"

class Ray
{
public:

	/**
	 \fn	Ray::Ray()
	
	 \brief	Default constructor.
	
	 \author	liuteng
	 \date	2016/3/12
	 */

	Ray();

	/**
	 \fn	Ray::Ray( const Point3f &Origin , const Vector3f &Direction , Float tMax = Infinity , Float time = 0.0f )
	
	 \brief	Constructor.
	
	 \author	liuteng
	 \date	2016/3/12
	
	 \param	Origin   	The origin.
	 \param	Direction	The direction.
	 \param	tMax	 	The maximum.
	 \param	time	 	The time.
	 */

	Ray( const Point3f &Origin , const Vector3f &Direction , float tMin , float tMax = Infinity , float time = 0.0f , int depth = 0 );

	/**
	 \fn	Point3f Ray::operator()( Float t ) const
	
	 \brief	Function call operator.
	
	 \author	liuteng
	 \date	2016/3/12
	
	 \param	t	length.
	
	 \return	沿着Direction与Origin的距离为t的点.
	 */

	Point3f operator()( float t ) const;

	/**
	 \fn	Ray::Ray( const Point3f& Origin , const Vector3f& Direction , const Ray& parent , float tmin , float tmax = Infinity );
	
	 \brief	Constructor.
	
	 \author	
	 \date	2016/4/8
	
	 \param	Origin   	The origin.
	 \param	Direction	The direction.
	 \param	parent   	The parent ray.
	 \param	tmin	 	The tmin.
	 \param	tmax	 	The tmax.
	 */

	Ray( const Point3f& Origin , const Vector3f& Direction , const Ray& parent , float tmin , float tmax = Infinity );

	/**
	 \fn	friend std::ostream& Ray::operator<< (std::ostream &os, const Ray &ray)
	
	 \brief	Stream insertion operator.
	
	 \author	liuteng
	 \date	2016/3/12
	
	 \param [in,out]	os	output stream.
	 \param	ray			  	The ray.
	
	 \return	ray information.
	 */

	friend std::ostream& operator << ( std::ostream &os , const Ray &ray );

	/**
	 \fn	Ray::Ray( const Ray& ray )
	
	 \brief	Copy constructor.
	
	 \author	liuteng
	 \date	2016/3/12
	
	 \param	ray	The ray.
	 */

	Ray( const Ray& ray );

public:
	Point3f Origin;
	Vector3f Direction;
	float time;
	mutable float MinT , MaxT;
	int depth;
};

#endif