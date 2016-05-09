#ifndef VERTEX_H
#define VERTEX_H

namespace DebugTextureEffect
{
	namespace VertexShader
	{
		struct cbPerFrame
		{
			DirectX::XMFLOAT4X4 WorldViewProj;
		};
	}
}

#endif