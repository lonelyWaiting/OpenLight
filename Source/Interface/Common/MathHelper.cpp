#include "MathHelper.h"
#include <float.h>
#include <cmath>

const float MathHelper::Infinity = FLT_MAX;		//浮点数最大值
const float MathHelper::Pi = 3.1415926535f;


float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	if (x >= 0.0f)
	{
		theta = atanf(y / x);	//求正切角

		if (theta < 0.0f)
		{
			theta += 2 * Pi;
		}
	}

	else
	{
		theta = atanf(y / x) + Pi;		//正切的周期是Pi
	}


	return theta;
}


DirectX::XMVECTOR MathHelper::RandHemisphereUnitVec3(DirectX::XMVECTOR n)
{
	DirectX::XMVECTOR One = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();


	while (true)
	{
		//生成一个在立方体[-1 , 1]^3内的随机点
		DirectX::XMVECTOR v = DirectX::XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f) , 0.0f);

		//如果生成的点在单位球外，则重新生成，防止其聚集在立方体的四个角
		if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(v), One))
			continue;

		//忽略聚集在中心的点
		if (DirectX::XMVector3Less(DirectX::XMVector3Dot(n, v), Zero))
			continue;

		//规格化
		return DirectX::XMVector3Normalize(v);
	}
}


DirectX::XMVECTOR MathHelper::RandUnitVec3()
{
	DirectX::XMVECTOR One = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	while (true)
	{
		//生成一个单位立方体中的随机点
		DirectX::XMVECTOR v = DirectX::XMVectorSet( MathHelper::RandF( -1.0f , 1.0f ) , MathHelper::RandF( -1.0f , 1.0f ) , MathHelper::RandF( -1.0f , 1.0f ) , 0.0f );

		//去除在单位球之外的点
		if ( DirectX::XMVector3Greater( DirectX::XMVector3LengthSq( v ) , One ) )
			continue;

		return DirectX::XMVector3Normalize( v );
	}
}