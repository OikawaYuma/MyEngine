#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include<cmath>
#include<cassert>
#include <vector>
#include <ModelData.h>
#include <stdlib.h>


// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

Vector3 MakeTransform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 Inverse(const Matrix4x4& m);

// 3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// 1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 2. 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

float Lerp(const float& v1, const float& v2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Quaternion Lerp(const Quaternion& v1, const Quaternion& v2, float t);
//Matrix4x4 MakeAffineMatrixBillboard(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
float Dot(const Vector3& v1, const Vector3& v2);
float DotQuaternion(const Quaternion& v1, const Quaternion& v2);
Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);
Quaternion SLerp(const Quaternion& v1, const Quaternion& v2, float t);
Vector3 Add(const Vector3& posa, const Vector3& posb);
Vector3 Subtract(const Vector3& posa, const Vector3& posb);

Vector3 Normalize(const Vector3& v);
Quaternion Normalize(Quaternion quaternion);
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time);
Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time);
Matrix4x4 Transpose(const Matrix4x4 m);
Vector3 Transform1(const Vector3& vector, const Matrix4x4& matrix);

float Length(const Vector3& v);

bool IsCollisionAABB(const Vector3& AABBPos, const Vector3& AABBMin, const Vector3& AABBMax, const Vector3& spherePos, const float& radius);