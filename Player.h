#pragma

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4.h"
#include "PlayerBullet.h"


// 自キャラ
class Player {

public: // メンバ関数
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
	PlayerBullet* bullet_ = nullptr;

};

