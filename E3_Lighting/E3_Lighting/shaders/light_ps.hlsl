// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float4 ambientColour;
	float4 specularColour;
	float specularPower;
	float3 position;
	float3 direction;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float3 viewVector: TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}


float4 calculateSpecular(float3 lightDirection, float3 viewVector, float3 normal)
{
	float3 halfway = normalize(lightDirection + viewVector);
	float intensity = pow(max(dot(normal, halfway), 0.0f), specularPower);
	
	return specularColour * intensity;
}


float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);

	float3 lightVector = normalize(position - input.worldPosition);
	////lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour) + ambientColour;
	lightColour = calculateLighting(lightVector, input.normal, diffuseColour) + ambientColour;
	
	//return lightColour * textureColour;
	////return lightColour;

	lightColour *= textureColour;
	lightColour += calculateSpecular(lightVector, input.viewVector, input.normal);
	//lightColour += calculateSpecular(-direction, input.viewVector, input.normal);
	return lightColour;
}



