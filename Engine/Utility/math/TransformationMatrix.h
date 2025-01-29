/**
* @ file TransformationMatrix.h
* @ brief Shaderに送るTransformationMatrixの構造体
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Matrix4x4.h"

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};
