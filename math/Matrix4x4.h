#pragma once
#include "Vector3.h"
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

// 回転移動行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// x軸回転行列
Matrix4x4 MakeRotationXMatrix(float theta);

// y軸回転行列
Matrix4x4 MakeRotationYMatrix(float theta);

// z軸回転行列
Matrix4x4 MakeRotationZMatrix(float theta);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(Vector3 trans);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 正規化
Vector3 Normalize(const Vector3& vector);

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

//Vector3& operator*=(Vector3& v, float s);
//
//Vector3 operator*(const Vector3& v, float s);