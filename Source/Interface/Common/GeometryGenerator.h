#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include "d3dUtil.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv)
			:Position(p), Normal(n), TangentU(t), TexCoord(uv)
		{}

		Vertex(float px, float py, float pz,
				float nx, float ny, float nz,
				float tx, float ty, float tz,
				float u, float v)
				:Position(px, py, pz),
				Normal(nx, ny, nz),
				TangentU(tx, ty, tz),
				TexCoord(u, v)
				{}

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 TangentU;
		DirectX::XMFLOAT2 TexCoord;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;		//vertex buffer
		std::vector<UINT> Indices;			//index buffer
	};


	//create a box centered at the origin with the given dimensions.
	void CreateBox(float width, float height, float depth, MeshData& meshData);

	
	//create a sphere centered at the origin with the given radius
	//slices and stacks parameters control the degree of tessllation
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);

	
	//create a geosphere centered at the origin with the given radius.
	//The depth controls the level of tessellation.
	void CreateGeoSphere(float radius, UINT numSubdivisions, MeshData& meshData);

	
	//Creates a cylinder parallel to the y-axis, and centered about the origin.
	//The bottom and top radius can vary to form various cone shapes rather than true
	//cylidners . The slices and stacks parameters control the degree of tessellation
	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);

	//create an m * n grid in the xz-plane with m rows and n columns,
	//at the origin with the speified width and depth
	void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);

	//Create a quad covering the screen in NDC coordinates. This is useful for
	//postprocessing effects.
	void CreateFullscreenQuad(MeshData& meshData);

private:
	void Subdivide(MeshData& meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
};

#endif