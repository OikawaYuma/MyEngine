#include "Object3d.hlsli"

struct Object3dForGPU {
	float32_t4x4 WVP;
	float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
    float32_t4 color;
};


StructuredBuffer<Object3dForGPU> gTransformationMatrix : register(t0);



struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
	VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix[instanceId].WVP);
	output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix[instanceId].WorldInverseTranspose));
    output.worldPosition = mul(input.position, gTransformationMatrix[instanceId].World).xyz;
    //output.color = gTransformationMatrix[instanceId].color;
    output.color = gTransformationMatrix[instanceId].color;
	return output;
}
//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}