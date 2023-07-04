#include "Matrix4x4.h"
#include <cmath>
#include <cassert>

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float a = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	          m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	          m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

	          m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	          m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	          m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

	          m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	          m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	          m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

	          m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	          m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	          m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

	          m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	          m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	          m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

	          m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	          m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	          m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

	          m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	          m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	          m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

	          m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	          m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	          m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	//assert(a != 1);
	a = 1.0f / a;
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	                  m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	                  m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) *
	                 a;
	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
	                  m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	                  m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) *
	                 a;
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	                  m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	                  m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) *
	                 a;
	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
	                  m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	                  m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) *
	                 a;

	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
	                  m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	                  m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][2]) *
	                 a;
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	                  m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	                  m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) *
	                 a;
	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
	                  m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	                  m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) *
	                 a;
	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	                  m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	                  m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) *
	                 a;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	                  m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	                  m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) *
	                 a;
	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
	                  m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	                  m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][1]) *
	                 a;
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	                  m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	                  m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) *
	                 a;
	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
	                  m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	                  m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) *
	                 a;

	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
	                  m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	                  m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) *
	                 a;
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	                  m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	                  m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) *
	                 a;
	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
	                  m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	                  m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) *
	                 a;
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	                  m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	                  m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) *
	                 a;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;

	result = {
		scale.x, 0.0f, 0.0f, 0.0f, 
		0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f, 
		0.0f, 0.0f,    0.0f, 1.0f};
	return result;
}

Matrix4x4 MakeRotationXMatrix(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {
	    1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos, sin, 0.0f,
		0.0f, -sin, cos, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 MakeRotationYMatrix(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {
	    cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin, 0.0f, cos, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 MakeRotationZMatrix(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;

	result = {
	    cos, sin, 0.0f, 0.0f,
		-sin, cos, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 MakeTranslateMatrix(Vector3 trans) {
	Matrix4x4 result = {
	    1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
		trans.x, trans.y, trans.z, 1.0f,
	};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans) {
	// スケーリング行列
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	// 回転行列
	Matrix4x4 matRotX = MakeRotationXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotationYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotationZMatrix(rot.z);
	// 合成(Z * X * Y)
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列
	Matrix4x4 matTrans = MakeTranslateMatrix(trans);

	// 合成
	Matrix4x4 result = matScale * matRot * matTrans;

	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};

	return result;
}


Vector3 Normalize(const Vector3& vector) {

	float length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	assert(length);
	Vector3 dir = {vector.x / length, vector.y / length, vector.z / length};
	return dir;
}

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2) { 
	Vector3 result = {};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	v1 = result;
	return v1;
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) { 
	Vector3 result = v1;

	return result -= v2;
}

//Vector3& operator*=(Vector3& v, float s) {
//
//}
//
//Vector3 operator*(const Vector3& v, float s) {
//
//}