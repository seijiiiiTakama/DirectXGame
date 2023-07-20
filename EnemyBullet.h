#pragma once

#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() { isDead_ = true; };

	Vector3 GetWorldPosition() {
		// ワールド座標を入れる変数
		Vector3 worldPos;
		// ワールド行列の平行移動成分を取得
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	};

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	// 弾
	//std::list<EnemyBullet*> bullets_;
};
