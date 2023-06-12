#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffinMatrix(scale, rotation, translation);
	// 定数バッファに転送する
	TransferMatrix();
};