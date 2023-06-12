#include "PlayerBullet.h"
#include "cassert"

void PlayerBullet::Initialize(Model* model, const Vector3& position){

	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	worldTransform_.Initialize();

	// 引き数で受け取った初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};

};

void PlayerBullet::Update(){

	//worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

};

void PlayerBullet::Draw(const ViewProjection& viewProjection){

	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

};