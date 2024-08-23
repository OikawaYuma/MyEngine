#pragma once
#include <cstdint>

// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;

// 敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
const uint32_t kCollisionAttributeItem = 0b1 << 2;
const uint32_t kCollisionAttributeplayerBullet = 0b1 << 2;


