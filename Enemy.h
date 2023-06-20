#pragma

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4.h"
#include "EnemyBullet.h"
#include <list>

// 自機キャラの前方宣言
class Player;

class Enemy {

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

public: // メンバ関数

	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	void ApproachInitialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void ApproachUpdate(Vector3& move);
	void LeaveUpdate(Vector3& move);

	void Fire();
	// 発射感覚
	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition(){
		// ワールド座標を入れる変数
		Vector3 worldPos;
		// ワールド行列の平行移動成分を取得
		worldPos.x = worldTransform_.translation_.x;
		worldPos.y = worldTransform_.translation_.y;
		worldPos.z = worldTransform_.translation_.z;

		return worldPos;
	};

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

private: // メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	std::list<EnemyBullet*> bullets_;
	// 発射タイマー
	int32_t fireTimer_ = 0;

	// 自キャラ
	Player* player_ = nullptr;

	Vector3 playerPos_;
	Vector3 enemyPos_;
	Vector3 difference_;
};


