#pragma

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4.h"
#include "PlayerBullet.h"
#include <list>


// 自キャラ
class Player {

public: // メンバ関数
	
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition() {
		// ワールド座標を入れる変数
		Vector3 worldPos;
		// ワールド行列の平行移動成分を取得
		worldPos.x = worldTransform_.translation_.x;
		worldPos.y = worldTransform_.translation_.y;
		worldPos.z = worldTransform_.translation_.z;

		return worldPos;
	};

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision(){};
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	Vector3 bulletPos_ = {};

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

private: // メンバ変数
	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;
	
	
};

