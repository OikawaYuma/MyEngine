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

// SkinninfObject3d.VS.hlsl�ō�������̂Ɠ���Palette
StructuredBuffer<Well> gMaterialPalette : register(t0);
// VertexBufferView��stream0�Ƃ��ė��p���Ă������͒��_
StructuredBuffer<Vertex> gInputVertices : register(t1);
// VertexBufferView��stream1�Ƃ��ė��p���Ă������̓C���t���G���X
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
// Skinning�v�Z��̒��_�f�[�^�BSkinningVertex
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
// Skinnning�Ɋւ��邿����Ƃ������
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID )
{
    
    uint32_t vertexIndex = DTid.x;
    if (vertexIndex < gSkinningInformation.numVertices)
    {
        // Skinning�v�Z
        // �K�v�ȃf�[�^��StructuredBuffer�������Ă���B
        // SkinningObject3d.VS�ł͓��͒��_�Ƃ��Ď󂯎���Ă���
        Vertex input = gInputVertices[vertexIndex];
        VertexInfluence influence = gInfluences[vertexIndex];
        
        // Skinning��̒��_���v�Z
        Vertex skinned;
        skinned.texcoord = input.texcoord;
        
        // �v�Z�̕��@��SkinningObject3d.VS�Ɠ���
        // �f�[�^�̎擾���@���ς�邾���ŁA�������ς��킯�ł͂Ȃ�
        // �ʒu�̕ϊ�
        skinned.position = mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.x].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.x;
        skinned.position += mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.y].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.y;
        skinned.position += mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.z].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.z;
        skinned.position += mul(input.position, gMatrixPalette[gInfluences[vertexIndex].index.w].skeletonSpaceMatrix) * gInfluences[vertexIndex].weight.w;
        skinned.position.w = 1.0f; // �m����1.0f�������
    
    // �@���̕ϊ�
        skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.x].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.x;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.y].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.y;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.z].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.z;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[gInfluences[vertexIndex].index.w].skeletonSpaceInverseTransposeMatrix) * gInfluences[vertexIndex].weight.w;
        skinned.normal = normalize(skinned.normal); // ���K�����Ė߂��Ă�����
        
        // Skinning��̃f�[�^���i�[�A�܂菑�����ށB
        gOutputVertices[vertexIndex] = skinned;
    }
    

}