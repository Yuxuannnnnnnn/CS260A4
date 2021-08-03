#pragma once

struct Vector3;
struct Quaternion;

#ifdef _MSC_VER
// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

union Matrix4x4
{
	struct
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};

	float m[16];
	float m2[4][4];

	static Matrix4x4 Identity();

	Matrix4x4();
	Matrix4x4(
		float, float, float, float,
		float, float, float, float,
		float, float, float, float,
		float, float, float, float);
	Matrix4x4(const float* arr);
	Matrix4x4(const Matrix4x4& copy);
	Matrix4x4& operator=(const Matrix4x4& rhs);
	Matrix4x4& operator *=(const Matrix4x4& rhs);
	Matrix4x4 operator*(const Matrix4x4& rhs);
	Vector3 operator*(const Vector3& rhs);

	static Matrix4x4 Rotate(Quaternion q);
	static Matrix4x4 Rotate(Vector3 v);
	static Matrix4x4 Scale(Vector3 v);
	static Matrix4x4 Scale(float f);
	static Matrix4x4 Translate(Vector3 v);
	static Matrix4x4 Transpose(Matrix4x4 m);

	static Matrix4x4 Ortho(
		float left, float right,
		float bottom, float top,
		float zNear, float zFar);
	static Matrix4x4 Perspective(
		float fov, float aspect,
		float zNear, float zFar);
};

#ifdef _MSC_VER
// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
