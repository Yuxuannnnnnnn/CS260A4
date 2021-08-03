#include "Math.h"
#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"

Matrix4x4 Matrix4x4::Identity()
{
	return Matrix4x4
	{
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
};

Matrix4x4::Matrix4x4() :
	m00{ 1 }, m01{ 0 }, m02{ 0 }, m03{ 0 },
	m10{ 0 }, m11{ 1 }, m12{ 0 }, m13{ 0 },
	m20{ 0 }, m21{ 0 }, m22{ 1 }, m23{ 0 },
	m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ 1 } {}

Matrix4x4::Matrix4x4(float v00, float v01, float v02, float v03,
	float v10, float v11, float v12, float v13,
	float v20, float v21, float v22, float v23,
	float v30, float v31, float v32, float v33) :
	m00{ v00 }, m01{ v01 }, m02{ v02 }, m03{ v03 },
	m10{ v10 }, m11{ v11 }, m12{ v12 }, m13{ v13 },
	m20{ v20 }, m21{ v21 }, m22{ v22 }, m23{ v23 },
	m30{ v30 }, m31{ v31 }, m32{ v32 }, m33{ v33 } {}

Matrix4x4::Matrix4x4(const Matrix4x4& copy) :
	m00{ copy.m00 }, m01{ copy.m01 }, m02{ copy.m02 }, m03{ copy.m03 },
	m10{ copy.m10 }, m11{ copy.m11 }, m12{ copy.m12 }, m13{ copy.m13 },
	m20{ copy.m20 }, m21{ copy.m21 }, m22{ copy.m22 }, m23{ copy.m23 },
	m30{ copy.m30 }, m31{ copy.m31 }, m32{ copy.m32 }, m33{ copy.m33 } {}

Matrix4x4::Matrix4x4(const float* arr) :
	m00{ arr[0] }, m01{ arr[1] }, m02{ arr[2] }, m03{ arr[3] },
	m10{ arr[4] }, m11{ arr[5] }, m12{ arr[6] }, m13{ arr[7] },
	m20{ arr[8] }, m21{ arr[9] }, m22{ arr[10] }, m23{ arr[11] },
	m30{ arr[12] }, m31{ arr[13] }, m32{ arr[14] }, m33{ arr[15] } {}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
{
	m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02; m03 = rhs.m03;
	m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13;
	m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23;
	m30 = rhs.m30; m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33;
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	// set the left hand side to be this for calculations
	Matrix4x4 lhs{ m };
	m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
	m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
	m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
	m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;
	m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
	m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
	m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
	m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;
	m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
	m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
	m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
	m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;
	m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
	m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
	m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
	m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;
	return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs)
{
	Matrix4x4 lhs{ m };
	return lhs *= rhs;
}

Vector3 Matrix4x4::operator*(const Vector3& rhs)
{
	//assumes homogeneous coordinates (translation matrices)
	Matrix4x4 lhs{ m };
	return Vector3
	{
	  lhs.m00 * rhs.x + lhs.m01 * rhs.y + lhs.m02 * rhs.z + lhs.m03,
	  lhs.m10 * rhs.x + lhs.m11 * rhs.y + lhs.m12 * rhs.z + lhs.m13,
	  lhs.m20 * rhs.x + lhs.m21 * rhs.y + lhs.m22 * rhs.z + lhs.m23
	};
}

Matrix4x4 Matrix4x4::Rotate(const Quaternion q)
{
	return Matrix4x4
	{
	  2 * q.w * q.w + 2 * q.x * q.x - 1, 2 * q.x * q.y + 2 * q.w * q.z,     2 * q.x * q.z - 2 * q.w * q.y,     0,
	  2 * q.x * q.y - 2 * q.w * q.z,     2 * q.w * q.w + 2 * q.y * q.y - 1, 2 * q.y * q.z + 2 * q.w * q.x,     0,
	  2 * q.x * q.z + 2 * q.w * q.y,     2 * q.y * q.z - 2 * q.w * q.x,     2 * q.w * q.w + 2 * q.z * q.z - 1, 0,
	  0, 0, 0, 1
	};
}

Matrix4x4 Matrix4x4::Rotate(const Vector3 v)
{
	float pi = 3.14159265358979323846f;
	float x = v.x / 180 * pi;
	float y = v.y / 180 * pi;
	float z = v.z / 180 * pi;
	// yaw pitch roll
	return Matrix4x4
	{
	  1, 0, 0, 0,
	  0, cosf(x), sinf(x), 0,
	  0, -sinf(x), cosf(x), 0,
	  0, 0, 0, 1
	} *
		Matrix4x4
	{
	  cosf(y), 0, -sinf(y), 0,
	  0, 1, 0, 0,
	  sinf(y), 0, cosf(y), 0,
	  0, 0, 0, 1
	} *
		Matrix4x4
	{
	  cosf(z), sinf(z), 0, 0,
	  -sinf(z), cosf(z), 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
}

Matrix4x4 Matrix4x4::Scale(const Vector3 v)
{
	return Matrix4x4
	{
	  v.x, 0, 0, 0,
	  0, v.y, 0, 0,
	  0, 0, v.z, 0,
	  0, 0, 0, 1
	};
}

Matrix4x4 Matrix4x4::Scale(const float f)
{
	return Matrix4x4
	{
	  f, 0, 0, 0,
	  0, f, 0, 0,
	  0, 0, f, 0,
	  0, 0, 0, 1
	};
}

Matrix4x4 Matrix4x4::Translate(const Vector3 v)
{
	return Matrix4x4
	{
	  1, 0, 0, v.x,
	  0, 1, 0, v.y,
	  0, 0, 1, v.z,
	  0, 0, 0, 1
	};
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4 m)
{
	return Matrix4x4
	{
	  m.m00, m.m10, m.m20, m.m30,
	  m.m01, m.m11, m.m21, m.m31,
	  m.m02, m.m12, m.m22, m.m32,
	  m.m03, m.m13, m.m23, m.m33
	};
}

Matrix4x4 Matrix4x4::Ortho(
	const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar)
{
	return Matrix4x4
	{
	  2 / (right - left), 0, 0, 0,
	  0, 2 / (top - bottom), 0, 0,
	  0, 0, 2 / (zNear - zFar), 0,
	  -(right + left) / (right - left), -(top + bottom) / (top - bottom), (zNear + zFar) / (zNear - zFar), 1
	};
}

Matrix4x4 Matrix4x4::Perspective(
	const float fov, const float aspect,
	const float zNear, const float zFar)
{
	float f = 1.0f / tanf(fov * Math::pi / 360); // convert degrees to radians and divide by 2
	return Matrix4x4
	{
	  f / aspect, 0, 0, 0,
	  0, f, 0, 0,
	  0, 0, (zFar + zNear) / (zNear - zFar), -1,
	  0, 0, 2 * zFar * zNear / (zNear - zFar), 0
	};
}