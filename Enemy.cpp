#include "Enemy.h"
#include "cassert"
#include "Player.h"
#include "cmath"
#include "GameScene.h"

//Enemy::~Enemy() {
//	
//}

void Enemy::Initialize(Model* model, Vector3 pos, Vector3 velocity) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.translation_.x = pos.x;
	worldTransform_.translation_.y = pos.y;
	worldTransform_.translation_.z = pos.z;
	kCharacterSpeed_ = velocity;

	// 接近フェーズ初期化
	ApproachInitialize();

	
}

void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	fireTimer_ = 60;
}


void Enemy::Update() {

	// キャラクターの移動ベクトル
	Vector3 move = kCharacterSpeed_;

	// 行列を計算、定数バッファに転送
	worldTransform_.UpdateMatrix();
	
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
	if (worldTransform_.translation_.z < 50.0f) {
		phase_ = Phase::Leave;
	}

	// 発射タイマーカウントダウン
	--fireTimer_;
	// 指定時間に達した
	if (fireTimer_ <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::LeaveUpdate(Vector3& move) {
	// 移動（ベクトルを加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += -move.y;
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, 0);

	// 自キャラのワールド座標を取得する
	playerPos_ = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	enemyPos_ = GetWorldPosition();
	// 敵キャラ→自キャラの差分ベクトルを求める
	difference_.x = enemyPos_.x - playerPos_.x;
	difference_.y = enemyPos_.y - playerPos_.y;
	difference_.z = enemyPos_.z - playerPos_.z;
	// ベクトルの正規化
	difference_ = Normalize(difference_);
	// ベクトルの長さを、早さに合わせる
	velocity.x = difference_.x * kBulletSpeed;
	velocity.y = difference_.y * kBulletSpeed;
	velocity.z = difference_.z * kBulletSpeed;

	//// 速度ベクトルを時期の向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//// 弾描画
	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}

}