#include "Precompiled.h"
#include "EngineMath.h"

using namespace SAVAGE::Math;

// TODO: define the rest of the statics here

const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f, 1.0f };
const Vector2 Vector2::XAxis{ 1.0f, 0.0f };
const Vector2 Vector2::YAxis{ 0.0f, 1.0f };

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f, 1.0f, 1.0f };
const Vector3 Vector3::XAxis{ 1.0f, 0.0f, 0.0f };
const Vector3 Vector3::YAxis{ 0.0f, 1.0f, 0.0f };
const Vector3 Vector3::ZAxis{ 0.0f, 0.0f, 1.0f };

const Quaternion Quaternion::Zero{ 0.0f, 0.0f, 0.0f, 0.0f };
const Quaternion Quaternion::Identity{ 1.0f, 0.0f, 0.0f, 0.0f };

Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return {
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f,
	};
}

Quaternion Quaternion::RotationAxis(const Vector3& axis, float t)
{
	const float c = cos(t * 0.5f);
	const float s = sin(t * 0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(c, a.x * s, a.y * s, a.z * s);
}

Quaternion Quaternion::RotationMatrix(const Matrix4& m)
{
	/*Quaternion q0;

	int i = -1, j = -1, k = -1;

	q0.w = sqrt(1 + m._11 + m._22 + m._33) / 2.0f;
	q0.x = (m._32 - m._23) / (4.0f * q0.w) * i;
	q0.y = (m._13 - m._31) / (4.0f * q0.w) * j;
	q0.z = (m._21 - m._12) / (4.0f * q0.w) * k;

	return Normalize(q0);*/

	float trace = m._11 + m._22 + m._33;
	if (trace > 0.0f)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		return
		{
			0.25f / s,
			(m._23 - m._32) * s,
			(m._31 - m._13) * s,
			(m._12 - m._21) * s
		};
	}
	else if (m._11 > m._22 && m._11 > m._33)
	{
		float s = 2.0f * sqrtf(1.0f + m._11 - m._22 - m._33);
		return
		{
			(m._23 - m._32) / s,
			0.25f * s,
			(m._21 + m._12) / s,
			(m._31 + m._13) / s
		};
	}
	else if (m._22 > m._33)
	{
		float s = 2.0f * sqrtf(1.0f + m._22 - m._11 - m._33);
		return
		{
			(m._31 - m._13) / s,
			(m._21 + m._12) / s,
			0.25f * s,
			(m._32 + m._23) / s
		};
	}
	else
	{
		float s = 2.0f * sqrtf(1.0f + m._33 - m._11 - m._22);
		return
		{
			(m._12 - m._21) / s,
			(m._31 + m._13) / s,
			(m._32 + m._23) / s,
			0.25f * s
		};
	}

}
Quaternion Quaternion::RotationLook(const Vector3& direction, const Vector3& up)
{
	Vector3 z = Normalize(direction);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	return Normalize(RotationMatrix(mat));
}
Quaternion Quaternion::RotationFromTo(const Vector3& from, const Vector3& to)
{
	Vector3 v1 = Normalize(from);
	Vector3 v2 = Normalize(to);

	float angle = acosf(Dot(v1, v2));
	Vector3 axis = Normalize(Cross(v1, v2));

	return Normalize(RotationAxis(axis, angle));
}


Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return Matrix4
	(
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}
