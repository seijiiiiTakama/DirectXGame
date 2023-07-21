#include "Player.h"
#include "cassert"
#include <imgui.h>
#include "WinApp.h"

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(
    Model* model, uint32_t textureHandle, uint32_t textureReticle, Vector3& position) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// テクスチャの受け取り
	textureHandle_ = textureHandle;
	textureReticle_ = textureReticle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();	

	worldTransform_.translation_ = position;

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	// スプライト取得
	sprite2DReticle_ =
	    Sprite::Create(textureReticle_, {0, 0}, {1, 1, 1, 1}, {0.5, 0.5}, false, false);
}

void Player::Update(ViewProjection viewProjection) {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} 

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 移動処理
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// 移動限界座標
	const float kMoveLimitX = 20;
	const float kMoveLimitY = 10;

	// 範囲を超えない世界
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列を計算、定数バッファに転送
	worldTransform_.UpdateMatrix();
	
	// キャラクターの座標を画面表示する処理
	/*ImGui::Begin(" ");
	ImGui::Text("Player %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();*/

	// 旋回処理
	Rotate();

	// キャラクター攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = {
		    worldTransform3DReticle_.matWorld_.m[3][0],
		    worldTransform3DReticle_.matWorld_.m[3][1],
		    worldTransform3DReticle_.matWorld_.m[3][2],
		};

		// ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, 1280, 720, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
		    viewProjection_.matView * viewProjection_.matProjection * matViewport;

		// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		// スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		// 自機か3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		// 自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = Normalize(offset) * kDistancePlayerTo3DReticle;
		// 3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;
		worldTransform3DReticle_.UpdateMatrix();
	}

	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{
		POINT mousePosition;
		// マウス座標(スクリーン座標)を取得する
		GetCursorPos(&mousePosition);

		// クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		// マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));

		// ビューポート行列
		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		// ビュープロジェクションビューポート合成行列
		Matrix4x4 matVPV =
		    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
		// 合成行列の逆行列を計算する
		Matrix4x4 matInverseVPV = Inverse(matVPV);

		// スクリーン座標
		Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
		Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);

		// スクリーン座標系からワールド座標系へ
		posNear = Transform(posNear, matInverseVPV);
		posFar = Transform(posFar, matInverseVPV);

		// マウスレイの方向
		Vector3 mouseDirection;
		mouseDirection.x = posNear.x - posFar.x;
		mouseDirection.y = posNear.y - posFar.y;
		mouseDirection.z = posNear.z - posFar.z;
		mouseDirection = Normalize(mouseDirection);
		// カメラから標準オブジェクトの距離
		const float kDistanceTestObject = -80.0f;
		worldTransform3DReticle_.translation_ = posNear + (mouseDirection * kDistanceTestObject);
		worldTransform3DReticle_.UpdateMatrix();

		ImGui::Begin("Player");
		ImGui::Text("2DReticle:(%f,%f)", float(mousePosition.x), float(mousePosition.y));
		ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
		ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
		ImGui::Text(
		    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
		ImGui::End();
	}
	
}

void Player::Rotate() {

	// 回転速さ［ラジアン/frame]
	const float kRotSpeed = 0.52f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		//worldTransform_.rotation_.x -= 3;
		worldTransform_.rotation_.y -= kRotSpeed;
		//worldTransform_.rotation_.z += 1;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = {0, 0, kBulletSpeed};

		// 速度ベクトルを時期の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 自機から標準オブジェクトへのベクトル
		velocity = Get3DReticlePosition() - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(
		    model_, GetWorldPosition(),
		    velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() {

	// 2Dレティクルを描画
	sprite2DReticle_->Draw();
}