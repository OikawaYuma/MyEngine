/**
* @ file SpotLight.h
* @ brief スポットライト用の構造体
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Vector4.h"
#include "Vector3.h"
struct SpotLight
{
	Vector4 color; // !< ライトの色
	Vector3 position; //!< ライトの位置
	float intensity; //!< 輝度
	Vector3 direction; //!< スポットライトの方向
	float distance; //!< ライトの届く最大距離
	float dacya; //!< 減衰率
	float cosAngle; //!< スポットライトの余弦
	float padding[2];
};

