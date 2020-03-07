// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightPosition;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
	float padding;
	float padding2;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculatePoint(float3 lightDirection, float3 normal, float4 diffuse, float3 pxWorldPosition)
{
	float3 pxToLightVector = pxWorldPosition - lightPosition;
	float distance = length(pxToLightVector);

	float intensity = saturate(dot(normal, lightDirection));
	//float4 colour = saturate(diffuse * intensity);

	float attenuation = 1 / (attenuationConstant + (attenuationLinear*distance) + (attenuationQuadratic*distance*distance));
	//float attenuation = 1 / (1.0f + (0.125f*distance) + (0.0f*distance*distance));
	float4 colour = saturate((diffuse * intensity) * attenuation);

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	float4 textureColour = texture0.Sample(sampler0, input.tex);
	float3 lightVector = normalize(lightPosition - input.worldPosition);
	float4 lightColour = ambientColour + calculatePoint(lightVector, input.normal, diffuseColour, input.worldPosition);
	
	return lightColour * textureColour;
}



