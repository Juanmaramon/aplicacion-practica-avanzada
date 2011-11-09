// Shader code from http://www.catalinzima.com/tutorials/4-uses-of-vtf/terrain-rendering-using-heightmaps/

//-----------------------------------------------------------------------------
// Variables del efecto
//-----------------------------------------------------------------------------

// Matriz WVP
float4x4 worldViewProj;
// Matrix World
float4x4 world;

// maximum height of the terrain
float maxHeight = 128;           

// this texture will point to the heightmap
texture displacementMap;    

//this sampler will be used to read (sample) the heightmap   
sampler displacementSampler = sampler_state {    
	Texture = <displacementMap>;
	MipFilter = Point;
	MinFilter = Point;
	MagFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT {
    float4 position : POSITION;
    float4 uv : TEXCOORD0;
};

struct VS_OUTPUT {
    float4 position  : POSITION;
    float4 uv : TEXCOORD0;
    float4 worldPos : TEXCOORD1;
};

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
VS_OUTPUT Transform(VS_INPUT In) {

    //initialize the output structure
    VS_OUTPUT Out = (VS_OUTPUT)0;                                  

    // this instruction reads from the heightmap, the value at the corresponding texture coordinate
    // Note: we selected level 0 for the mipmap parameter of tex2Dlod, since we want to read data exactly as it appears in the heightmap
    float height = tex2Dlod ( displacementSampler, float4(In.uv.xy , 0 , 0 ) );

    // with the newly read height, we compute the new value of the Y coordinate
    // we multiply the height, which is in the (0,1) range by a value representing the Maximum Height of the Terrain
    In.position.y = height * maxHeight;

    //Pass the world position the the Pixel Shader
    Out.worldPos = mul(In.position, world);

    //Compute the final projected position by multiplying with the world, view and projection matrices                                                      
    Out.position = mul(In.position , worldViewProj);

    Out.uv = In.uv;

    return Out;

}

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
float4 PixelShader(in float4 worldPos : TEXCOORD1) : COLOR {

        return worldPos.y / maxHeight;

}

//-----------------------------------------------------------------------------
// Simple Effect (1 technique with 1 pass)
//-----------------------------------------------------------------------------
technique Technique0 {

    pass Pass0 {
        VertexShader = compile vs_3_0 Transform();
        PixelShader  = compile ps_3_0 PixelShader();
    }

}