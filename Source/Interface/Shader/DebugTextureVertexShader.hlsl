struct VertexIn
{
    float3 PosL     :       POSITION;
    float2 Tex      :       TEXCOORD;
};

struct VertexOut
{
    float4 PosH     :       SV_POSITION;
    float2 Tex      :       TEXCOORD;
};


VertexOut main( VertexIn input )
{
    VertexOut Output;

	Output.PosH = float4( input.PosL , 1.0f );
	Output.Tex = input.Tex;

    return Output;
}