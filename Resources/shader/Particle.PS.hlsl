//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}
#include "Particle.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
    float32_t shininess;
};
struct DirectionalLight {
	float32_t4 color; //!< ライトの色
	float32_t3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{

	PixelShaderOutput output;
    if (output.color.a == 0.0)
    {
        discard;
    }
    if (input.color.a == 0.0)
    {
        discard;
    }
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    if (textureColor.a == 0.0)
    {
        discard;
    }
	output.color = gMaterial.color * textureColor * input.color;
	if (gMaterial.enableLighting != 0) {
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		//output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		//output.color.a = gMaterial.color.a * textureColor.a;
        float32_t nl = max(0, dot(input.normal, -gDirectionalLight.direction)) * gMaterial.shininess;
        if (nl <= 0.01f)
        {
            nl = 0.3f;
        }
        else if (nl <= 0.3f)
        {
            nl = 0.5f;
        }
        else
        {
            nl = 1.0f;
        }
        output.color.rgb *= nl;
        output.color.a = gMaterial.color.a * textureColor.a * input.color.a;
    }
	else {
		output.color = gMaterial.color * textureColor;
	}
 
	return output;

};