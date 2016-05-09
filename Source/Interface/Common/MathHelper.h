#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <Windows.h>
#include <DirectXMath.h>

class MathHelper
{
public:
	//random float in [0,1)
	static float RandF()
	{
        return (float)( rand() ) / (float)RAND_MAX;
	}

	//rand float in [a , b]
	static float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + t * (b - a);
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	//return the polar angle of the point (x , y) in [0 , 2 * PI]
	static float AngleFromXY(float x, float y);

	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		//Inverse-transpose is just applied to normals . 
		//So zero out translation row so that it doesn't get into 
		//our inverse-transpose calculation -- we don't want 
		//the inverse-transpose of the translation
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);	//最后一行是用来控制平移的，而法线是向量，平移对其没有意义

		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);	//求其行列式

		return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	}



	static DirectX::XMVECTOR RandUnitVec3();
	static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

	static const float Infinity;		//无限
	static const float Pi;
};

#endif