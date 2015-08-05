

//GLOBALS

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//TYPEDEFS
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


//VERTEX SHADER
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	//change position vector to be 4 units for proper matrix calculation 
	input.position.w = 1.0f;
	
	//calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;
	
	return output;
}