#include "Enemy.h"
#include "cassert"

void Enemy::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.translation_.y = 5;
}

void Enemy::Update() {

	// キャラクターの移動速さ
	const float kCharacterSpeed = -0.2f;

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, kCharacterSpeed};
	
	// 移動処理
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// 行列を計算、定数バッファに転送
	// worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

}

void Enemy::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}