/**
* @ file VertexData.h
* @ brief VertexDataの構造体
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

struct VertexData {
	Vector4 position;
	Vector2 texcorrd;
	Vector3 normal;
};
