#include "GeometryGenerator.h"
#include "MathHelper.h"



void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
{
	//Create the vertices , the number of triangle is six
	Vertex v[24];

	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;

	
	//Fill in the front face vertex data
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(w2, h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//Fill in the back face  vertex data
	v[4] = Vertex( w2 , -h2 , d2 , 0.0f , 0.0f , 1.0f , -1.0f , 0.0f , 0.0f , 0.0f , 1.0f );
	v[5] = Vertex( w2 , h2 , d2 , 0.0f , 0.0f , 1.0f , -1.0f , 0.0f , 0.0f , 0.0f , 0.0f );
	v[6] = Vertex( -w2 , h2 , d2 , 0.0f , 0.0f , 1.0f , -1.0f , 0.0f , 0.0f , 1.0f , 0.0f );
	v[7] = Vertex( -w2 , -h2 , d2 , 0.0f , 0.0f , 1.0f , -1.0f , 0.0f , 0.0f , 1.0f , 1.0f );

	//Fill in the top face vertex data
	v[8] = Vertex(-w2, h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, h2, d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(w2, h2, d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(w2, h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//Fill in the bottom face vertex data
	v[12] = Vertex( w2 , -h2 , -d2 , 0.0f , -1.0f , 0.0 , -1.0f , 0.0f , 0.0f , 0.0f , 1.0f );
	v[13] = Vertex( w2 , -h2 , d2 , 0.0f , -1.0f , 0.0f , -1.0f , 0.0f , 0.0f , 0.0f , 0.0f );
	v[14] = Vertex( -w2 , -h2 , d2 , 0.0f , -1.0f , 0.0f , -1.0f , 0.0f , 0.0f , 1.0f , 0.0f );
	v[15] = Vertex( -w2 , -h2 , -d2 , 0.0f , -1.0f , 0.0f , -1.0f , 0.0f , 0.0f , 1.0f , 1.0f );

	//Fill in the left face vertex data
	v[16] = Vertex(-w2, -h2, d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, h2, d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	//Fill in the right face vertex data
	v[20] = Vertex(w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(w2, h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(w2, h2, d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(w2, -h2, d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.Vertices.assign(&v[0], &v[24]);

	

	//Create tgeh indices
	UINT i[36];

	//Fill in the font face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	//Fill in the bottom face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	//Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	//Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	//Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	//Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);
}



void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	
	Vertex topVertex(0.0f, radius, 0.0f,	0.0f, 1.0f,	0.0f,	1.0f, 0.0f,	0.0f,	0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius,0.0f,	0.0f, -1.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f);
	


	//将球从上到下横切成stackCount片
	//每个切面都是一个圆
	//然后我们将这个圆分为sliceCount份
	//然后求出每个点的坐标
	//通过球形坐标进行求解
	//使用球坐标需要知道半径，与竖直轴的正轴的夹角
	//在底面的投影与x的夹角
	meshData.Vertices.push_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;			//与竖直轴的正轴的夹角，这里是与y轴正轴的夹角
		
		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;	//在底面的投影与x轴的夹角
			Vertex v;

			v.Position.x = radius * sinf(phi) * cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi) * sinf(theta);


			/*
				可以通过由割线无限逼近切线，并且结合极限，来进行推导曲面上的一点的切线向量
				可参见：http://netedu.xauat.edu.cn/jpkc/netedu/jpkc/gdsx/homepage/5jxsd/51/513/5308/530806.htm
				因此：Position对于theta的偏导数作为切线坐标
			*/
			v.TangentU.x = -radius * sinf(phi) * sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = radius * sinf(phi) * cosf(theta);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&v.TangentU);
			DirectX::XMStoreFloat3(&v.TangentU, DirectX::XMVector3Normalize(T));


			//法线即为圆心指向该点的向量，因此刚好等于点的位置坐标的值
			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&v.Position);
			DirectX::XMStoreFloat3(&v.Normal, DirectX::XMVector3Normalize(p));

			v.TexCoord.x = theta / DirectX::XM_2PI;
			v.TexCoord.y = phi / DirectX::XM_PI;

			meshData.Vertices.push_back(v);
		}
	}

	meshData.Vertices.push_back(bottomVertex);




	//compute indices for top stack . The top Stack was written first to the vertex buffer
	//and connects the top pole to the first ring
	//将最顶上那个圆环的相邻的两个点与最高的顶点相连组成三角形
	for (UINT i = 1; i <= sliceCount; i++)
	{
		//注意方向，由于i+1的点距离x轴正轴偏离的角度比第i个点偏离的角度更大，因此按照顺时针的原则应该是0，i+1，i
		meshData.Indices.push_back(0);
		meshData.Indices.push_back( i + 1 );
		meshData.Indices.push_back(i);
		
	}


	//处理内部的环，不包括最后一个环，因为最后一个环上的点是和最底部的顶点组成三角形
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;

	for (UINT i = 0; i < stackCount - 2; i++)		//遍历中间的stackCount-2个环
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			//将在当前环的索引值为j,j+1的两个点和上一个环中索引值想同的两个点
			//相连，组成两个三角形，由于最高点的索引值为0，所以从第一个环开始
			//第i个环的起始索引值为（i - 1）*(sliceCount + 1)，
			//注意是第i个环，所以后面才是i-1，循环的时候从0开始，
			//所以计算的时候不需要减一
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}


	//最后一个环
	UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

	//在最后一个环中第一个顶点的索引偏移值
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; i++)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}



void GeometryGenerator::Subdivide(MeshData& meshData)
{
	//保存一份输入图元
	MeshData inputCopy = meshData;

	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = inputCopy.Indices.size() / 3;	//每三个索引组成一个三角形

	for (UINT i = 0; i < numTris; i++)
	{
		Vertex v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
		Vertex v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
		Vertex v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];


		//生成中心坐标
		Vertex m0, m1, m2;

		//我们只关心位置坐标
		m0.Position = DirectX::XMFLOAT3((v0.Position.x + v1.Position.x) * 0.5f,
								(v0.Position.y + v1.Position.y) * 0.5f,
								(v0.Position.z + v1.Position.z) * 0.5f);

		m1.Position = DirectX::XMFLOAT3((v1.Position.x + v2.Position.x) * 0.5f,
								(v1.Position.y + v2.Position.y) * 0.5f,
								(v1.Position.z + v2.Position.z) * 0.5f);

		m2.Position = DirectX::XMFLOAT3((v2.Position.x + v0.Position.x) * 0.5f,
								(v2.Position.y + v0.Position.y) * 0.5f,
								(v2.Position.z + v0.Position.z) * 0.5f);


		meshData.Vertices.push_back(v0);
		meshData.Vertices.push_back(v1);
		meshData.Vertices.push_back(v2);
		meshData.Vertices.push_back(m0);
		meshData.Vertices.push_back(m1);
		meshData.Vertices.push_back(m2);


		meshData.Indices.push_back(6 * i + 0);
		meshData.Indices.push_back(6 * i + 3);
		meshData.Indices.push_back(6 * i + 5);

		meshData.Indices.push_back(6 * i + 3);
		meshData.Indices.push_back(6 * i + 4);
		meshData.Indices.push_back(6 * i + 5);
		
		meshData.Indices.push_back(6 * i + 5);
		meshData.Indices.push_back(6 * i + 4);
		meshData.Indices.push_back(6 * i + 2);

		meshData.Indices.push_back(6 * i + 3);
		meshData.Indices.push_back(6 * i + 1);
		meshData.Indices.push_back(6 * i + 4);
	}
}



//将一个二十面体细分，使其逼近球
//将组成二十面体的每个三角形进行细分
//细分后的结果，通过将其规格化为单位球
//按照球面坐标来推导切线坐标，法线坐标，纹理坐标等
void GeometryGenerator::CreateGeoSphere(float radius, UINT numSubdivisions, MeshData& meshData)
{
	numSubdivisions = MathHelper::Min(numSubdivisions , 5u);	//设置细分上限

	const float X = 0.525731f;
	const float Z = 0.850651f;


	//构建一个20面体，20面体总共12个顶点
	//索引值为60个
	DirectX::XMFLOAT3 pos[12] = 
	{
		DirectX::XMFLOAT3(-X, 0.0f, Z),  DirectX::XMFLOAT3(X, 0.0f, Z),  
		DirectX::XMFLOAT3(-X, 0.0f, -Z), DirectX::XMFLOAT3(X, 0.0f, -Z),    
		DirectX::XMFLOAT3(0.0f, Z, X),   DirectX::XMFLOAT3(0.0f, Z, -X), 
		DirectX::XMFLOAT3(0.0f, -Z, X),  DirectX::XMFLOAT3(0.0f, -Z, -X),    
		DirectX::XMFLOAT3(Z, X, 0.0f),   DirectX::XMFLOAT3(-Z, X, 0.0f), 
		DirectX::XMFLOAT3(Z, -X, 0.0f),  DirectX::XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	meshData.Vertices.resize(12);
	meshData.Vertices.resize(60);


	for(UINT i = 0; i < 12; i++)
	{
		meshData.Vertices[i].Position = pos[i];
	}

	for(UINT i = 0; i < 60; i++)
	{
		meshData.Indices[i] = k[i];
	}

	//细分二十面体
	for(UINT i = 0; i < numSubdivisions; i++)
	{
		Subdivide(meshData);
	}

	//为细分后的逼近球计算各个顶点的位置，法线，切线，纹理坐标
	for(UINT i = 0; i < meshData.Vertices.size(); i++)
	{
		//将其规格化为单位球
		DirectX::XMVECTOR n = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&meshData.Vertices[i].Position));

		DirectX::XMVECTOR p = DirectX::XMVectorScale( n , radius );		//由于球的半径为radius，且n为单位向量，所以直接相乘就得到了

		DirectX::XMStoreFloat3(&meshData.Vertices[i].Position , p);


		//法线
		DirectX::XMStoreFloat3(&meshData.Vertices[i].Normal , n);


		//利用球坐标推导纹理坐标及切线向量
		//求出球坐标的两个角度
		float theta = MathHelper::AngleFromXY(meshData.Vertices[i].Position.x , meshData.Vertices[i].Position.z);

		float phi = acosf(meshData.Vertices[i].Position.y / radius);


		//纹理坐标
		meshData.Vertices[i].TexCoord.x = theta / DirectX::XM_2PI;
		meshData.Vertices[i].TexCoord.y = phi / DirectX::XM_PI;


		//切线坐标，位置关于theta的偏导数
		meshData.Vertices[i].TangentU.x = -radius * sinf(phi) * sinf(theta);
		meshData.Vertices[i].TangentU.y = 0.0f;
		meshData.Vertices[i].TangentU.z = radius * sinf(phi) * cosf(theta);

		DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&meshData.Vertices[i].TangentU);
		DirectX::XMStoreFloat3(&meshData.Vertices[i].TangentU , DirectX::XMVector3Normalize(T));
	}
}



void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();


	//将圆柱体从上到下分stackCount份，每一份绕着中心分sliceCount
	float stackHeight = height / stackCount;	//y轴的增量

	double radiusStep = (topRadius - bottomRadius) / stackCount;	//半径增量

	UINT ringCount = stackCount + 1;
	for(UINT  i = 0; i < ringCount; i++)
	{
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		//圆环分割后角度的增量
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;

		for(UINT j = 0; j <= sliceCount; j++)		//圆环上分割的sliceCount份
		{
			Vertex vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			vertex.Position = DirectX::XMFLOAT3(r * c , y , r * s);

			vertex.TexCoord.x = (float)j / sliceCount;
			vertex.TexCoord.y = 1.0f - (float)i / stackCount;	//由于是从下往上分割的

			

			//考虑这种情况：
			// y(v) = h - hv for v in [0,1]
			// r(v) = r1 + (r0 - r1) * v
			//
			// x(t , v) = r(v) * cos(t)
			// y(t , v) = h - hv;
			// z(t , v) = r(v) * sin(t)
			//
			// dx/dt = -r(v)*sin(t);
			// dy/dt = 0;
			// dz/dt = r(v)*cos(t)
			//
			// dx/dv = (r0 - r1)*cos(t)
			// dy/dv = -h;
			// dz/dv = (r0 - r1)*sin(t);

			vertex.TangentU = DirectX::XMFLOAT3(-s , 0.0f , c);

			float dr = topRadius - bottomRadius;
			DirectX::XMFLOAT3 bitTangent(-dr * c , -height , -dr * s);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&vertex.TangentU);
			DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&bitTangent);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T , B));

			DirectX::XMStoreFloat3(&vertex.Normal , N);

			meshData.Vertices.push_back(vertex);
		}
	}


	//Index
	UINT ringVertexCount = sliceCount + 1;
	for(UINT i = 0; i < stackCount; i++)
	{
		for(UINT j = 0; j < sliceCount; j++)
		{
			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			meshData.Indices.push_back(i * ringVertexCount + j + 1);
		}
	}


	BuildCylinderTopCap(bottomRadius , topRadius , height , sliceCount , stackCount , meshData);
	BuildCylinderBottomCap(bottomRadius , topRadius , height , sliceCount , stackCount , meshData);
}


void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	UINT baseIndex = (UINT)meshData.Vertices.size();

	float y = 0.5f * height;
	float dTheta = DirectX::XM_2PI / sliceCount;

	for(UINT i = 0; i <= sliceCount; i++)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.Vertices.push_back(Vertex(x , y , z , 0.0f , 1.0f , 0.0f , 1.0f , 0.0f , 0.0f , u , v));
	}

	//上顶面的中心点
	meshData.Vertices.push_back(Vertex(0.0f , y , 0.0f , 0.0f , 1.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f , 0.0f));


	//上顶面中心点的索引值
	UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

	for(UINT i = 0; i < sliceCount; i++)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}


void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	UINT baseIndex = meshData.Vertices.size();

	float y = -0.5f * height;
	float dTheta = DirectX::XM_2PI / sliceCount;

	for(UINT i = 0; i <= sliceCount; i++)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.Vertices.push_back(Vertex(x , y , z , 0.0f , -1.0f , 0.0f , 1.0f , 0.0f , 0.0f , u , v));
	}

	meshData.Vertices.push_back(Vertex(0.0f , -0.5f * height , 0.0f , 0.0f , -1.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.5f , 0.5f));

	UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

	for(UINT i = 0; i < sliceCount; i++)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}


void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
	UINT vertexCount = m * n;		//m行n列的网格
	UINT faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = width * 0.5f;
	float halfDepth = depth * 0.5f;

	float xStep = width / (n - 1);
	float zStep = depth / (m - 1);

	meshData.Vertices.resize(vertexCount);
	meshData.Indices.resize(faceCount * 3);

	float du = 1.0f / (n -1);
	float dv = 1.0f / (m - 1);

	for(UINT i = 0; i < m; i++)
	{
		float z = halfDepth - i * zStep;

		for(UINT j = 0; j < n; j++)
		{
			float x = -halfWidth + j * xStep;

			meshData.Vertices[i * n + j].Position = DirectX::XMFLOAT3(x , 0.0f , z);
			meshData.Vertices[i * n + j].Normal = DirectX::XMFLOAT3(0.0f , 1.0f , 0.0f);
			meshData.Vertices[i * n + j].TangentU = DirectX::XMFLOAT3(1.0f , 0.0f , 0.0f);
			meshData.Vertices[i * n + j].TexCoord = DirectX::XMFLOAT2(j * du , i * dv);
		}
	}


	UINT k = 0;
	for(UINT i = 0; i < m - 1; i++)
	{
		for(UINT j = 0; j < n - 1; j++)
		{
			meshData.Indices[k] = i * n + j;
			meshData.Indices[k + 1] = i * n + j + 1;
			meshData.Indices[k + 2] = (i + 1) * n + j;

			meshData.Indices[k + 3] = (i + 1) * n + j;
			meshData.Indices[k + 4] = i * n + j + 1;
			meshData.Indices[k + 5] = (i + 1) * n + j + 1;

			k += 6;
		}
	}
}



void GeometryGenerator::CreateFullscreenQuad(MeshData& meshData)
{
	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	meshData.Vertices[0] = Vertex(-1.0f, -1.0f, 0.5f, 
                                   0.0f, 0.0f, -1.0f, 
                                   1.0f, 0.0f, 0.0f, 
                                   0.0f, 1.0f);
	meshData.Vertices[1] = Vertex(-1.0f, 1.0f, 0.5f, 
                                   0.0f, 0.0f, -1.0f, 
                                   1.0f, 0.0f, 0.0f, 
                                   0.0f, 0.0f);
	meshData.Vertices[2] = Vertex(1.0f, 1.0f, 0.5f, 
                                   0.0f, 0.0f, -1.0f, 
                                   1.0f, 0.0f, 0.0f, 
                                   1.0f, 0.0f);
	meshData.Vertices[3] = Vertex(1.0f, -1.0f, 0.5f, 
                                   0.0f, 0.0f, -1.0f, 
                                   1.0f, 0.0f, 0.0f, 
                                   1.0f, 1.0f);


	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;
}