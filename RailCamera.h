#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Matrix4x4.h"
#include <ImGuiManager.h>
#include "DebugCamera.h"


/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/*Matrix4x4 GetMatView() { return viewProjection_.matView; }
	Matrix4x4 GetMatProjection() { return viewProjection_.matProjection; }
	void SetMatView(Matrix4x4 matview) { this->viewProjection_.matView = matview; } 
	void SetMatProjection(Matrix4x4 matprojection) { this->viewProjection_.matProjection = matprojection; } */

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ImGui
	float translationFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float rotationFloat3[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, 
		worldTransform_.rotation_.z};

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
};
