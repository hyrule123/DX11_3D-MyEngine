#ifndef POSTPROCESS_HLSLI
#define POSTPROCESS_HLSLI

#include "Engine/DefaultShader/Commons.hlsli"

struct VSIn
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD;
};

#endif
