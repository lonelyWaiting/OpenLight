struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Direction;
    float Spot;

    float3 Att;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight( Material mat , DirectionalLight L ,
                              float3 normal , float3 toEye ,
                              out float4 ambient ,
                              out float4 diffuse ,
                              out float4 spec )
{
    // Initialize outputs.
    ambient = float4( 0.0f , 0.0f , 0.0f , 0.0f );
    diffuse = float4( 0.0f , 0.0f , 0.0f , 0.0f );
    spec = float4( 0.0f , 0.0f , 0.0f , 0.0f );

    // The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;

        // Add ambient term.
        ambient = mat.Ambient * L.Ambient;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuseFactor = dot( lightVec , normal );

    // Flatten to avoid dynamic branching.
    [flatten]
    if ( diffuseFactor > 0.0f )
    {
        float3 v = reflect( -lightVec , normal );
            float specFactor = pow( max( dot( v , toEye ) , 0.0f ) , mat.Specular.w );

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight( Material mat , PointLight L , float3 pos , float3 normal , float3 toEye ,
                        out float4 ambient , out float4 diffuse , out float4 spec )
{
    // Initialize outputs.
    ambient = float4( 0.0f , 0.0f , 0.0f , 0.0f );
    diffuse = float4( 0.0f , 0.0f , 0.0f , 0.0f );
    spec = float4( 0.0f , 0.0f , 0.0f , 0.0f );

    // The vector from the surface to the light.
    float3 lightVec = L.Position - pos;

        // The distance from surface to light.
        float d = length( lightVec );

    // Range test.
    if ( d > L.Range )
        return;

    // Normalize the light vector.
    lightVec /= d;

    // Ambient term.
    ambient = mat.Ambient * L.Ambient;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuseFactor = dot( lightVec , normal );

    // Flatten to avoid dynamic branching.
    [flatten]
    if ( diffuseFactor > 0.0f )
    {
        float3 v = reflect( -lightVec , normal );
            float specFactor = pow( max( dot( v , toEye ) , 0.0f ) , mat.Specular.w );

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }

    // Attenuate
    float att = 1.0f / dot( L.Att , float3( 1.0f , d , d*d ) );

    diffuse *= att;
    spec *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight( Material mat , SpotLight L , float3 pos , float3 normal , float3 toEye ,
                       out float4 ambient , out float4 diffuse , out float4 spec )
{
    // Initialize outputs.
    ambient = float4( 0.0f , 0.0f , 0.0f , 0.0f );
    diffuse = float4( 0.0f , 0.0f , 0.0f , 0.0f );
    spec = float4( 0.0f , 0.0f , 0.0f , 0.0f );

    // The vector from the surface to the light.
    float3 lightVec = L.Position - pos;

        // The distance from surface to light.
        float d = length( lightVec );

    // Range test.
    if ( d > L.Range )
        return;

    // Normalize the light vector.
    lightVec /= d;

    // Ambient term.
    ambient = mat.Ambient * L.Ambient;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuseFactor = dot( lightVec , normal );

    // Flatten to avoid dynamic branching.
    [flatten]
    if ( diffuseFactor > 0.0f )
    {
        float3 v = reflect( -lightVec , normal );
            float specFactor = pow( max( dot( v , toEye ) , 0.0f ) , mat.Specular.w );

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }

    // Scale by spotlight factor and attenuate.
    float spot = pow( max( dot( -lightVec , L.Direction ) , 0.0f ) , L.Spot );

    // Scale by spotlight factor and attenuate.
    float att = spot / dot( L.Att , float3( 1.0f , d , d*d ) );

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

//-----------------------------------------------------------------------------------------
//根据法线贴图采样的值计算出世界空间中物体的真实法线
//法线贴图中记录的是该点的切线空间中的坐标
//由于我们的光线计算是在世界空间中，因此我们需要计算出
//在世界空间中该点的真实法线
//-----------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace( float3 normalMapSample , float3 unitNormalW , float3 tangentW )
{
    //法线贴图中采样的值范围在[0 , 1]，然而我们需要将其转换到[-1 , 1]
    float3 NormalTangent = 2 * normalMapSample - 1;


        //正交坐标轴
        float3 N = unitNormalW;
        float3 T = normalize( tangentW - dot( tangentW , N ) * N );
        float3 B = cross( N , T );


        /*
        Tx Ty Tz
        Bx By Bz
        Nx Ny Nz
        */
        float3x3 TBN = float3x3( T , B , N );

        float3 BumpedNormalW = mul( NormalTangent , TBN );

        return BumpedNormalW;
}


static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;


//
// 计算阴影系数：
// shadowPosH是以光源为视点时，该点从局部空间变换到纹理空间后的坐标值
// (局部->世界->视图->NDC->纹理空间)
// shadowPosH.xy代表纹理坐标
// shadowPosH.z代表在以灯光为视点时，归一化的深度值
// 
// 1.0代表完全不在阴影中，0.0f代表完全处于阴影中
//
float CalcShadowFactor( SamplerComparisonState samShadow ,
                        Texture2D shadowMap ,
                        float4 shadowPosH )
{
    shadowPosH.xyz /= shadowPosH.w;

    // 在Light Space中该点的深度值
    float depth = shadowPosH.z;

    const float dx = SMAP_DX;
    
    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2( -dx , -dx )  ,  float2( 0.0f , -dx )  , float2( dx , -dx )  ,
        float2( -dx , 0.0f ) ,  float2( 0.0f , 0.0f ) , float2( dx , 0.0f ) ,
        float2( -dx , dx )   ,  float2( 0.0f , dx )   , float2( dx , dx )
    };


    [unroll]
    for ( int i = 0; i < 9; i++ )
    {
        // 
        // shadowMap.SampleCmpLevelZero(SampleComparisonState samShadow , float2 pos , float depth)
        // 以pos为纹理坐标采样，采样方式由samShadow定义
        // 采样得到的值是一个深度值s
        // 和depth进行比较,比较操作有samShadow的成员ComparisonFunc定义
        // 如：当ComparisonFunc定义的操作为LESS
        // 则当depth < s 时，返回1
        // 否则返回0
        //
        percentLit += shadowMap.SampleCmpLevelZero( samShadow , shadowPosH.xy + offsets[i] , depth ).r;
    }

    return percentLit /= 9.0f;
}