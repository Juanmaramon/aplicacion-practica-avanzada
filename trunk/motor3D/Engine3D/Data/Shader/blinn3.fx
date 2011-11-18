
//-----------------------------------------------------------------------------
// Variables del efecto
//-----------------------------------------------------------------------------

// Matriz WVP
float4x4 worldViewProj;
float4x4 world;
float4x4 worldInverseTranspose;

// Direccion de la luz
float3 LightDirection : Direction = float3(0,50,10);

// Color de la luz de ambiente
float4 AmbientColor : Ambient = float4(.1,.1,.1,1);

// Intensidad de la luz
float AmbientIntensity = 1;

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
	float3 Normal : NORMAL;
};

// Datos de salida del vertex shader
struct VS_OUTPUT
{
    	float4 position : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 Light : TEXCOORD1;
	float3 Normal : TEXCOORD2;
};

// Salida del pixel shader
struct PS_OUTPUT
{
	float4 Color : COLOR;
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

	// Diffuse lightning
	OUT.Light = normalize(LightDirection);
	OUT.Normal = normalize(mul((float3x3)worldInverseTranspose, IN.Normal));

	return OUT;
}


//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
PS_OUTPUT myps( VS_OUTPUT IN )
{
	PS_OUTPUT output;
	
	float Diffuse = saturate(dot(IN.Light, IN.Normal));
	float4 Ambient = AmbientIntensity * AmbientColor;
	float4 texCol = tex2D(Diffuse_0, IN.tex0);
	texCol *= Diffuse;
	output.Color = Ambient + texCol;

	return output;
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
