//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}
#include "Object3d.hlsli"

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
};
struct DirectionalLight
{
    float32_t4 color; //!< ライトの色
    float32_t3 direction; //!< ライトの向き
    float intensity; //!< 輝度
    
};

struct SpotLight
{
    float32_t4 color; //!< ライトの色
    float32_t3 position; //!< ライトの位置
    float32_t intensity; //!< 輝度
    float32_t3 direction; //!< スポットの方向
    float32_t distance; //!< ライトの届く最大距離
    float32_t decay; //!< 減衰率
    float32_t cosAngle; //!< スポットライトの余弦
    
};
struct Camera
{
    float32_t3 worldPosition;
};
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<SpotLight> gSpotLight : register(b3);
Texture2D<float32_t4> gTexture : register(t0);
//TextureCube<float32_t4> gEnvironmentTexture : register(t1);

SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
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
    output.color = gMaterial.color * textureColor* input.color;
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        //float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        float32_t3 directionHalfVector = normalize(-gDirectionalLight.direction + toEye);
        //float RdotE = dot(reflectLight, toEye);
        float32_t directionNDotE = dot(normalize(input.normal), directionHalfVector);
        float32_t directionSpecularPow = pow(saturate(directionNDotE), gMaterial.shininess);; // 反射強度
        // 拡散反射
        float32_t3 directionDiffuse =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        // 鏡面反射
        float32_t3 directionSpecular =
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * directionSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        
        float32_t distance = length(gSpotLight.position - input.worldPosition); // ポイントライトへ距離
        float32_t factor = 3.0f / (distance * distance); // 逆二乗則による減衰係数
        float32_t cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (1.0f - gSpotLight.cosAngle));
        
        float32_t3 spotHalfVector = normalize(-gSpotLight.direction + toEye);
        //float RdotE = dot(reflectLight, toEye);
        float32_t spotNDotE = dot(normalize(input.normal), spotHalfVector);
        
        float32_t spotSpecularPow = pow(saturate(spotNDotE), 30.0f); // 反射強度
        
        float32_t3 spotDiffuse =
        gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cos * gSpotLight.intensity * falloffFactor * factor;
       
        float32_t3 spotSpecular =
        gSpotLight.color.rgb * gSpotLight.intensity * spotSpecularPow * float32_t3(0.1f, 0.1f, 0.1f) * falloffFactor * factor;
        // 拡散反射+鏡面反射
        
       // output.color.rgb *= gMaterial.color.rgb;
        
        //Objectの法線と光の方向と強さから輝度をとる
        float32_t nl = 
        max(0, dot(input.normal, -gDirectionalLight.direction)) * gMaterial.shininess;
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
        output.color.rgb *=nl;
       // output.color.rgb *= nl;
        
        // 以下はこの方法でもできるということ　attenuationFactorは距離による減衰のこと
        //gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor
        
        //// αは今まで通り
        //output.color.a = gMaterial.color.a * textureColor.a;
        //float32_t3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
        //float32_t3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
       // float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);

        //// 拡散反射+鏡面反射
        //output.color.rgb = environmentColor.rgb + diffuse + specular;
        //// αは今まで通り
        output.color.a = gMaterial.color.a * textureColor.a * input.color.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor * input.color.a;
    }
    
    return output;

};