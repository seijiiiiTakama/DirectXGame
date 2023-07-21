#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"


// 自キャラ
class Player {

public: // メンバ関数
	
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    Model* model, uint32_t textureHandle, uint32_t textureReticle, Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition() {
		// ワールド座標を入れる変数
		Vector3 worldPos;
		// ワールド行列の平行移動成分を取得
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	};

	Vector3 Get3DReticlePosition() {
		// ワールド座標を入れる変数
		Vector3 reticlePos;
		// ワールド行列の平行移動成分を取得
		reticlePos.x = worldTransform3DReticle_.matWorld_.m[3][0];
		reticlePos.y = worldTransform3DReticle_.matWorld_.m[3][1];
		reticlePos.z = worldTransform3DReticle_.matWorld_.m[3][2];

		return reticlePos;
	};

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision(){};
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	Vector3 bulletPos_ = {};

	/// <summary>
	/// 親となるワールドトランフォームをセット
	/// </summary>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private: // メンバ変数
	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;
	
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	// テクスチャハンドル
	uint32_t textureReticle_ = 0u;
};

