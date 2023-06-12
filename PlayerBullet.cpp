﻿#include "PlayerBullet.h"
#include "cassert"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
};

void PlayerBullet::Update(){

	//worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
};

void PlayerBullet::Draw(const ViewProjection& viewProjection){

	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

};