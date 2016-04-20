#include "PCH.h"
#include "Core/Scene.h"
#include "Math/Ray.h"
#include "Shapes/Shape.h"
#include "Grid.h"

void Grid::Setup( const Scene* scene )
{
	Bound3f SceneBBox = scene->GetWorldBoundingBox();

	float ObjectCount = scene->GetObjectCount();

	int Count = 0;

	// 目前不支持组合形状下挂接组合形状
	for( int i = 0; i < ObjectCount; i++ )
	{
		Primitive primitive = scene->GetPrimitive( i );

		for( int j = 0; j < primitive.GetShapeCount(); j++ )
		{
			Shape* shape = primitive.GetShape( j );

			if( shape->IsCombinationShape() )
			{
				Count += shape->GetSubShapeCount();
			}
			else
			{
				Count += 1;
			}
		}
	}

	// 计算scene bounding box沿着x , y , z轴的长度
	float wx = SceneBBox.pMax.x - SceneBBox.pMin.x;
	float wy = SceneBBox.pMax.y - SceneBBox.pMin.y;
	float wz = SceneBBox.pMax.z - SceneBBox.pMin.z;

	float lambda = 2.0f;

	float s = std::pow( wx * wy * wz / Count , 1.0f / 3.0f );

	nx = lambda * wx / s + 1;
	ny = lambda * wy / s + 1;
	nz = lambda * wz / s + 1;

	int CellsCount = nx * ny * nz;

	cells.reserve( CellsCount );

	for( int i = 0; i < CellsCount; i++ )
	{
		cells.push_back( nullptr );
	}

	// temporary array to hold the count of object store in each cells
	std::vector<int> Counts;
	Counts.reserve( CellsCount );

	for( int i = 0; i < CellsCount; i++ )
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

			if( shape->IsCombinationShape() )
			{
				// 组合形状
				for( int k = 0; shape->GetSubShapeCount(); k++ )
				{
					Shape* SubShape = shape->GetSubShape( k );

					BBox = SubShape->GetWorldBoundingBox();

					int iMinX = ( BBox.pMin.x - SceneBBox.pMin.x ) * nx / wx;
					int iMinY = ( BBox.pMin.y - SceneBBox.pMin.y ) * ny / wy;
					int iMinZ = ( BBox.pMin.z - SceneBBox.pMin.z ) * nz / wz;

					int iMaxX = ( BBox.pMax.x - SceneBBox.pMin.x ) * nx / wx;
					int iMaxY = ( BBox.pMax.y - SceneBBox.pMin.y ) * ny / wy;
					int iMaxZ = ( BBox.pMax.z - SceneBBox.pMax.z ) * nz / wz;

					Clamp( iMinX , 0 , nx - 1 );
					Clamp( iMinY , 0 , ny - 1 );
					Clamp( iMinZ , 0 , nz - 1 );

					Clamp( iMaxX , 0 , nx - 1 );
					Clamp( iMaxY , 0 , ny - 1 );
					Clamp( iMaxZ , 0 , nz - 1 );

					for( int iz = iMinZ; iz < iMaxZ; iz++ )
					{
						for( int iy = iMinY; iy < iMaxY; iy++ )
						{
							for( int ix = iMinX; ix < iMaxX; ix++ )
							{
								index = ix + nx * iy + iz * ny * nx;

								cells[index] = SubShape;
								Counts[index] += 1;
							}
						}
					}
				}
			}
			else
			{
				BBox = shape->GetWorldBoundingBox();

				int iMinX = ( BBox.pMin.x - SceneBBox.pMin.x ) * nx / wx;
				int iMinY = ( BBox.pMin.y - SceneBBox.pMin.y ) * ny / wy;
				int iMinZ = ( BBox.pMin.z - SceneBBox.pMin.z ) * nz / wz;

				int iMaxX = ( BBox.pMax.x - SceneBBox.pMin.x ) * nx / wx;
				int iMaxY = ( BBox.pMax.y - SceneBBox.pMin.y ) * ny / wy;
				int iMaxZ = ( BBox.pMax.z - SceneBBox.pMax.z ) * nz / wz;

				Clamp( iMinX , 0 , nx - 1 );
				Clamp( iMinY , 0 , ny - 1 );
				Clamp( iMinZ , 0 , nz - 1 );

				Clamp( iMaxX , 0 , nx - 1 );
				Clamp( iMaxY , 0 , ny - 1 );
				Clamp( iMaxZ , 0 , nz - 1 );

				for( int iz = iMinZ; iz < iMaxZ; iz++ )
				{
					for( int iy = iMinY; iy < iMaxY; iy++ )
					{
						for( int ix = iMinX; ix < iMaxX; ix++ )
						{
							index = ix + nx * iy + iz * ny * nx;

							cells[index] = shape;
							Counts[index] += 1;
						}
					}
				}
			}
		}
	}

	Counts.erase( Counts.begin() , Counts.end() );
}

bool Grid::Intersect( Ray& ray , Scene* scene , IntersectRecord* record )
{

}