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
	worldTransform_.translation_.z = 10;
}

void Enemy::Update() {

	// キャラクターの移動速さ
	const float kCharacterSpeed = -0.2f;

	// キャラクターの移動ベクトル
	Vector3 move = {kCharacterSpeed, kCharacterSpeed, kCharacterSpeed};

	// 行列を計算、定数バッファに転送
	// worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	switch(phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate(move);
		break;
	case Phase::Leave:
		LeaveUpdate(move);
		break;
	}
}

void Enemy::ApproachUpdate(Vector3& move) {
	// 移動（ベクトルを加算）
	worldTransform_.translation_.z += move.z;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate(Vector3& move) {
	// 移動（ベクトルを加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += -move.y;
}

void Enemy::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}