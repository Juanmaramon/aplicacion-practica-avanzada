//-----------------------------------------------------------------------------
// Variables del efecto
//-----------------------------------------------------------------------------

// Matriz WVP
float4x4 worldViewProj;

sampler2D Diffuse_0 = sampler_state {
	minFilter = LinearMipMapLinear;
	magFilter = Linear;
	AddressU = Repeat;
	AddressV = Repeat;
};

// Datos del vertice
struct VS_INPUT
{
    float3 position : POSITION;
	float2 tex0 	: TEXCOORD0;
};

// Datos de salida del vertex shader
struct VS_OUTPUT
{
    float4 position : POSITION;
	float2 tex0 : TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
VS_OUTPUT myvs( const VS_INPUT IN )
{
	VS_OUTPUT OUT;
	float4 position = float4(IN.position, 1.0);
	OUT.position = mul( worldViewProj, position );
	OUT.tex0 = IN.tex0;
	return OUT;
}


//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
float4 myps( VS_OUTPUT IN ): COLOR
{
	return tex2D(Diffuse_0, IN.tex0);
}

//-----------------------------------------------------------------------------
// Simple Effect (1 technique with 1 pass)
//-----------------------------------------------------------------------------
technique Technique0
{
    pass Pass0
    {
        Zenable  = true;
        CullFaceEnable = false;

        VertexShader = compile arbvp1 myvs();
        PixelShader  = compile arbfp1 myps();
    }
}
