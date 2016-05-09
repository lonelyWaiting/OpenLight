#include <Utilities/PCH.h>
#include <Core/Scene.h>
#include <Math/Ray.h>
#include <Shapes/Shape.h>
#include <Shapes/AggregateShape.h>
#include "Grid.h"

void Grid::Setup( const Scene* scene )
{
	Bound3f SceneBBox = scene->GetWorldBoundingBox();

	int ObjectCount = scene->GetObjectCount();

	int Count = 0;

	// 目前不支持组合形状下挂接组合形状
	for( int i = 0; i < ObjectCount; i++ )
	{
		Primitive primitive = scene->GetPrimitive( i );

		Count += primitive.GetShapeCount();
	}

	// 计算scene bounding box沿着x , y , z轴的长度
	double wx = SceneBBox.pMax.x - SceneBBox.pMin.x;
	double wy = SceneBBox.pMax.y - SceneBBox.pMin.y;
	double wz = SceneBBox.pMax.z - SceneBBox.pMin.z;

	double lambda = 2.0f;

	double s = std::pow( wx * wy * wz / Count , 1.0f / 3.0f );

	nx = static_cast< int >( lambda * wx / s + 1 );
	ny = static_cast< int >( lambda * wy / s + 1 );
	nz = static_cast< int >( lambda * wz / s + 1 );

	int CellsCount = nx * ny * nz;

	cells.reserve( CellsCount );
	for( int j = 0; j < CellsCount; j++ )
	{
		cells.push_back( nullptr );
	}

	// 用于临时记录各个Cell中图元的个数
	std::vector<int> Counts;
	Counts.reserve( CellsCount );
	for( int j = 0; j < CellsCount; j++ )
	{
		Counts.push_back( 0 );
	}

	Bound3f BBox;
	int index = -1;

	for( int i = 0; i < ObjectCount; i++ )
	{
		Primitive primitive = scene->GetPrimitive( i );

		for( int j = 0; j < primitive.GetShapeCount(); j++ )
		{
			Shape* shape = primitive.GetShape( j );

			BBox = shape->GetWorldBoundingBox();

			int iMinX = static_cast< int >( ( BBox.pMin.x - SceneBBox.pMin.x ) * nx / wx );
			int iMinY = static_cast< int >( ( BBox.pMin.y - SceneBBox.pMin.y ) * ny / wy );
			int iMinZ = static_cast< int >( ( BBox.pMin.z - SceneBBox.pMin.z ) * nz / wz );

			int iMaxX = static_cast< int >( ( BBox.pMax.x - SceneBBox.pMin.x ) * nx / wx );
			int iMaxY = static_cast< int >( ( BBox.pMax.y - SceneBBox.pMin.y ) * ny / wy );
			int iMaxZ = static_cast< int >( ( BBox.pMax.z - SceneBBox.pMin.z ) * nz / wz );

			Clamp( iMinX , 0 , nx - 1 );
			Clamp( iMinY , 0 , ny - 1 );
			Clamp( iMinZ , 0 , nz - 1 );

			Clamp( iMaxX , 0 , nx - 1 );
			Clamp( iMaxY , 0 , ny - 1 );
			Clamp( iMaxZ , 0 , nz - 1 );


			for( int iz = iMinZ; iz <= iMaxZ; iz++ )
			{
				for( int iy = iMinY; iy <= iMaxY; iy++ )
				{
					for( int ix = iMinX; ix <= iMaxX; ix++ )
					{
						index = ix + nx * iy + iz * ny * nx;

						if( Counts[index] == 0 )
						{
							cells[index] = shape;
							Counts[index] += 1;
						}
						else if( Counts[index] == 1 )
						{
							AggregateShape* pAggregateShape = new AggregateShape;
							pAggregateShape->AddShape( cells[index] );
							pAggregateShape->AddShape( shape );
							cells[index] = pAggregateShape;
							Counts[index] += 1;
						}
						else
						{
							( ( AggregateShape* )cells[index] )->AddShape( shape );
							Counts[index] += 1;
						}
					}
				}
			}
		}
	}

	Counts.erase( Counts.begin() , Counts.end() );
}

bool Grid::Intersect( Ray& ray , const Scene* scene , IntersectRecord* record )
{
	double ox = ray.Origin.x;
	double oy = ray.Origin.y;
	double oz = ray.Origin.z;

	double dx = ray.Direction.x;
	double dy = ray.Direction.y;
	double dz = ray.Direction.z;

	Bound3f SceneBoundingBox = scene->GetWorldBoundingBox();

	double x0 = SceneBoundingBox.pMin.x;
	double y0 = SceneBoundingBox.pMin.y;
	double z0 = SceneBoundingBox.pMin.z;

	double x1 = SceneBoundingBox.pMax.x;
	double y1 = SceneBoundingBox.pMax.y;
	double z1 = SceneBoundingBox.pMax.z;

	double txMin , tyMin , tzMin;
	double txMax , tyMax , tzMax;

	if( dx == 0.0 )
	{
		txMin = -Infinity;
		txMax = Infinity;
	}
	else
	{
		double Invdx = 1.0 / dx;

		if( Invdx > 0.0 )
		{
			txMin = ( x0 - ox ) * Invdx;
			txMax = ( x1 - ox ) * Invdx;
		}
		else
		{
			txMin = ( x1 - ox ) * Invdx;
			txMax = ( x0 - ox ) * Invdx;
		}
	}

	if( dy == 0.0 )
	{
		tyMin = -Infinity;
		tyMax = Infinity;
	}
	else
	{
		double Invdy = 1.0 / dy;

		if( Invdy > 0.0 )
		{
			tyMin = ( y0 - oy ) * Invdy;
			tyMax = ( y1 - oy ) * Invdy;
		}
		else
		{
			tyMin = ( y1 - oy ) * Invdy;
			tyMax = ( y0 - oy ) * Invdy;
		}
	}

	if( dz == 0.0 )
	{
		tzMin = -Infinity;
		tzMax = Infinity;
	}
	else
	{
		double Invdz = 1.0 / dz;

		if( Invdz > 0.0 )
		{
			tzMin = ( z0 - oz ) * Invdz;
			tzMax = ( z1 - oz ) * Invdz;
		}
		else
		{
			tzMin = ( z1 - oz ) * Invdz;
			tzMax = ( z0 - oz ) * Invdz;
		}
	}
	
	double t0 = txMin , t1 = txMax;

	t0 = ( t0 < tyMin ? tyMin : t0 ) < tzMin ? tzMin : ( t0 < tyMin ? tyMin : t0 );

	t1 = ( t1 > tyMax ? tyMax : t1 ) > tzMax ? tzMax : ( t1 > tyMax ? tyMax : t1 );

	if( t0 > t1 )
	{
		return false;
	}

	int ix , iy , iz;

	if( Inside( ray.Origin , SceneBoundingBox ) )
	{
		ix = static_cast< int >( ( ox - x0 ) / ( x1 - x0 ) * nx );
		iy = static_cast< int >( ( oy - y0 ) / ( y1 - y0 ) * ny );
		iz = static_cast< int >( ( oz - z0 ) / ( z1 - z0 ) * nz );

		Clamp( ix , 0 , nx - 1 );
		Clamp( iy , 0 , ny - 1 );
		Clamp( iz , 0 , nz - 1 );
	}
	else
	{
		Point3f p = ray( t0 );

		ix = static_cast< int >( ( p.x - x0 ) / ( x1 - x0 ) * nx );
		iy = static_cast< int >( ( p.y - y0 ) / ( y1 - y0 ) * ny );
		iz = static_cast< int >( ( p.z - z0 ) / ( z1 - z0 ) * nz );

		Clamp( ix , 0 , nx - 1 );
		Clamp( iy , 0 , ny - 1 );
		Clamp( iz , 0 , nz - 1 );
	}

	double dtx = ( txMax - txMin ) / nx;
	double dty = ( tyMax - tyMin ) / ny;
	double dtz = ( tzMax - tzMin ) / nz;

	double txNext , tyNext , tzNext;
	int ixStep , iyStep , izStep;
	int ixStop , iyStop , izStop;

	if( dx == 0.0 )
	{
		txNext = Infinity;
		ixStep = -1;
		ixStop = -1;
	}
	else
	{
		if( dx > 0 )
		{
			txNext = txMin + ( ix + 1 ) * dtx;
			ixStep = 1;
			ixStop = nx;
		}
		else
		{
			txNext = txMin + ( nx - ix ) * dtx;
			ixStep = -1;
			ixStop = -1;
		}
	}



	if( dy == 0.0 )
	{
		tyNext = Infinity;
		iyStep = -1;
		iyStop = -1;
	}
	else
	{
		if( dy > 0 )
		{
			tyNext = tyMin + ( iy + 1 ) * dty;
			iyStep = 1;
			iyStop = ny;
		}
		else
		{
			tyNext = tyMin + ( ny - iy ) * dty;
			iyStep = -1;
			iyStop = -1;
		}
	}


	if( dz == 0.0 )
	{
		tzNext = Infinity;
		izStep = -1;
		izStop = -1;
	}
	else
	{
		if( dz > 0 )
		{
			tzNext = tzMin + ( iz + 1 ) * dtz;
			izStep = 1;
			izStop = nz;
		}
		else
		{
			tzNext = tzMin + ( nz - iz ) * dtz;
			izStep = -1;
			izStop = -1;
		}
	}

	while( true )
	{
		Shape* shape = cells[ix + nx * iy + nx * ny * iz];

		bool bHit = false;

		if( txNext < tyNext && txNext < tzNext )
		{
			Ray r( ray.Origin , ray.Direction , ray.MinT , txNext , ray.time , ray.depth );

			if( shape )
			{
				bHit = shape->Intersect( r , record );
			}

			if( bHit )
			{
				return true;
			}

			txNext += dtx;
			ix += ixStep;

			if( ix == ixStop )
			{
				return false;
			}
		}
		else if( tyNext < tzNext )
		{
			Ray r( ray.Origin , ray.Direction , ray.MinT , tyNext , ray.time , ray.depth );

			if( shape )
			{
				bHit = shape->Intersect( r , record );
			}

			if( bHit )
			{
				return true;
			}

			tyNext += dty;
			iy += iyStep;

			if( iy == iyStop )
			{
				return false;
			}
		}
		else
		{
			Ray r( ray.Origin , ray.Direction , ray.MinT , tzNext , ray.time , ray.depth );

			if( shape )
			{
				bHit = shape->Intersect( r , record );
			}

			if( bHit )
			{
				return true;
			}

			tzNext += dtz;
			iz += izStep;

			if( iz == izStop )
			{
				return false;
			}
		}

	}
	
}