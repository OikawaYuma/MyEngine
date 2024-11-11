#include "SkinningObject3d.VS.hlsl"

struct Vertex
{
    float32_t4 position;
    float32_t2 texcoord;
    float32_t3 normal;
};

struct VertexInfluence
{
    float32_t4 weight;
    int32_t4 index;
};

struct SkinningInformation
{
    uint32_t numVertices;
};

// SkinninfObject3d.VS.hlslで作ったものと同じPalette
StructuredBuffer<Well> gMaterialPalette : register(t0);
// VertexBufferViewのstream0として利用していた入力頂点
StructuredBuffer<Vertex> gInputVertices : register(t1);
// VertexBufferViewのstream1として利用していた入力インフルエンス
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
// Skinning計算後の頂点データ。SkinningVertex
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
// Skinnningに関するちょっとした情報
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID )
{
    
    uint32_t vertexIndex = DTid.x;
    if (vertexIndex < gSkinningInformation.numVertices)
    {
        // Skinning計算
        // 必要なデータをStructuredBufferから取ってくる。
        // SkinningObject3d.VSでは入力頂点として受け取っていた
        Vertex input = gInputVertices[vertexIndex];
        VertexInfluence influence = gInfluences[vertexIndex];
        
        // Skinning後の頂点を計算
        Vertex skinned;
        skinned.texcoord = input.texcoord;
        
        // 計算の方法はSkinningObject3d.VSと同じ
        // データの取得方法が変わるだけで、原理が変わるわけではない
        // 位置の変換
        skinned.position = mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.x].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.x;
        skinned.position += mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.y].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.y;
        skinned.position += mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.z].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.z;
        skinned.position += mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.w].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.w;
        skinned.position.w = 1.0f; // 確実に1.0fをいれり
    
    // 法線の変換
        skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.x].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.x;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.y].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.y;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.z].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.z;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.w].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.w;
        skinned.normal = normalize(skinned.normal); // 正規化して戻してあげる
        
        // Skinning後のデータを格納、つまり書き込む。
        gOutputVertices[vertexIndex] = skinned;
    }
    

}