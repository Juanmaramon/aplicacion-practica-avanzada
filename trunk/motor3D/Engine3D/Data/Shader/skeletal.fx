//--------------------------------------------------------------------
// Variables del efecto
//--------------------------------------------------------------------

// Matriz WVP
#define MAX_BONES 80
uniform float4 BonesRow[ MAX_BONES * 3 ];
float4x4 worldViewProj;
float4x4 world;
float4x4 worldInverseTranspose;

// Direccion de la luz
float3 LightDirection : Direction = float3(0,50,10);

// Color de la luz de ambiente
float4 AmbientColor : Ambient = float4(.5,.5,.5,1);

// Intensidad de la luz
float AmbientIntensity = 1;

// Datos del vertice
struct VS_INPUT
{
	float3 position : POSITION;
	float4 indexes: TEXCOORD0;
	float4 weights: COLOR0;
	float3 Normal : NORMAL;
};

// Datos de salida del vertex shader
struct VS_OUTPUT
{
	float4 position : POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
};

// Salida del pixel shader
struct PS_OUTPUT
{
	float4 Color : COLOR;
};

//--------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------
VS_OUTPUT myvs( const VS_INPUT IN )
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	float4 tempPos;
	tempPos.xyz = IN.position.xyz;
	tempPos.w = 1.0;
	float base = IN.indexes.x * 3;
	float3x4 BoneMatrix = float3x4( BonesRow[ base ],
	BonesRow[base + 1], BonesRow[base + 2]);
	float3 pos = mul(BoneMatrix, tempPos) * IN.weights.x;
	base = IN.indexes.y * 3;
	BoneMatrix = float3x4( BonesRow[ base ], BonesRow[base + 1],
	BonesRow[base + 2]);
	pos += mul(BoneMatrix, tempPos) * IN.weights.y;
	base = IN.indexes.z * 3;
	BoneMatrix = float3x4( BonesRow[ base ], BonesRow[base + 1],
	BonesRow[base + 2]);
	pos += mul(BoneMatrix, tempPos) * IN.weights.z;
	base = IN.indexes.w * 3;
	BoneMatrix = float3x4( BonesRow[ base ], BonesRow[base + 1],
	BonesRow[base + 2]);
	pos += mul(BoneMatrix, tempPos) * IN.weights.w;
	OUT.position = mul( worldViewProj, float4( pos, 1.0 ) );	

	// Diffuse lightning
	OUT.Light = normalize(LightDirection);
	OUT.Normal = normalize(mul((float3x3)worldInverseTranspose, IN.Normal));

	return OUT;
}

//--------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------
PS_OUTPUT myps( VS_OUTPUT IN )
{
	PS_OUTPUT output;
	//output.Color = AmbientIntensity * AmbientColor;
	
	output.Color = (AmbientIntensity * AmbientColor) + saturate(dot(IN.Light, IN.Normal));
	return output;
}

//--------------------------------------------------------------------
// Simple Effect (1 technique with 1 pass)
//--------------------------------------------------------------------
technique Technique0
{
	pass Pass0
	{
		Zenable = true;
		CullFaceEnable = false;
		VertexShader = compile glslv myvs();
		PixelShader = compile glslf myps();
	}
}
