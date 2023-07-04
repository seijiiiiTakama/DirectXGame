#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate) {

	// ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() {

	worldTransform_.translation_.z += -0.1f;
	//worldTransform_.rotation_.y += 0.001f;

	// ワールド行列再計算
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();

	// カメラの座標を画面表示する処理
	/*ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", translationFloat3, 0.0f, 1.0f);
	ImGui::SliderFloat3("rotation", rotationFloat3, 0.0f, 1.0f);
	ImGui::End();*/


}