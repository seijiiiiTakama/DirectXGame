#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4.h"

class Enemy {

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void ApproachUpdate(Vector3& move);
	void LeaveUpdate(Vector3& move);

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

};


