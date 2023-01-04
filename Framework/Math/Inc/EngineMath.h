#pragma once

#include "Common.h"

#include "Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Random.h"

#include "AABB.h"
#include "Sphere.h"

namespace SAVAGE::Math
{
	template <class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template <class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	constexpr float Abs(float value)
	{
		return value >= 0.0f ? value : -value;
	}

	constexpr float Sqr(float value)
	{
		return value * value;
	}

	constexpr float Dot(const Vector3& v0, const Vector3& v1)
	{
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrt(Dot(v, v));
	}

	constexpr float DistanceSqr(const Vector3& v0, const Vector3& v1)
	{
		return MagnitudeSqr(v0 - v1);
	}

	inline float Distance(const Vector3& v0, const Vector3& v1)
	{
		return Magnitude(v0 - v1);
	}

	inline Vector3 Normalize(const Vector3& v)
	{
		float invMag = 1.0f / Magnitude(v);
		return v * invMag;
	}

	constexpr Vector3 Cross(const Vector3& u, const Vector3& v)
	{
		return {
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
		};
	}

	inline float Determinant(const Matrix4& m)
	{
		float det = 0.0f;
		det += (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
		det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
		det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
		det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
		return det;
	}

	inline Matrix4 Adjoint(const Matrix4& m)
	{
		return
		{
			+(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			+(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

			-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			+(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			+(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

			+(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			+(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

			-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			+(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			+(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		};
	}

	inline Matrix4 Transpose(const Matrix4& m)
	{
		return {
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		};
	}

	inline Matrix4 Inverse(const Matrix4& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}

	constexpr Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		const float w = ((v.x * m._14) + (v.y * m._24) + (v.z * m._34) + (1.0f * m._44));
		const float invW = (w == 0.0f) ? 1.0f : 1 / w;
		return {
			((v.x * m._11) + (v.y * m._21) + (v.z * m._31) + (1.0f * m._41)) * invW,
			((v.x * m._12) + (v.y * m._22) + (v.z * m._32) + (1.0f * m._42)) * invW,
			((v.x * m._13) + (v.y * m._23) + (v.z * m._33) + (1.0f * m._43)) * invW
		};
	}

	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		return {
			(v.x * m._11) + (v.y * m._21) + (v.z * m._31),
			(v.x * m._12) + (v.y * m._22) + (v.z * m._32),
			(v.x * m._13) + (v.y * m._23) + (v.z * m._33)
		};
	}

	/*bool Intersect(Ray ray, Sphere sphere, float& distance)
	{
		Vector3 v = sphere.center - ray.origin;
		float t = Dot(v, ray.direction);

		Vector3 closestPoint = ray.origin + (ray.direction * t);
		float closestPointToCenterSqr = MagnitudeSqr(closestPoint - sphere.center);
		float radiusSqr = sphere.radius * sphere.radius;
		if (closestPointToCenterSqr > radiusSqr)
			return false;

		float oppositeSide = sqrt(radiusSqr - closestPointToCenterSqr);
		if (MagnitudeSqr(ray.origin - sphere.center) < radiusSqr)
			distance = t + oppositeSide;
		else
			distance = t - oppositeSide;
		return true;
	}*/

	inline float Magnitude(const Quaternion& q)
	{
		return sqrt((q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z));
	}

	inline Quaternion Normalize(const Quaternion& q)
	{
		return q / Magnitude(q);
	}

	constexpr Quaternion Lerp(Quaternion q0, Quaternion q1, float t)
	{
		return q0 + ((q1 * t) - (q0 * t));
	}

	constexpr Quaternion Slerp(Quaternion q0, Quaternion q1, float t)
	{
		float dot = (q0.w * q1.w) + (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z);

		// Determine the direction of the rotation.
		if (dot < 0.0f)
		{
			dot = -dot;
			q1.w = -q1.w;
			q1.x = -q1.x;
			q1.y = -q1.y;
			q1.z = -q1.z;
		}
		else if (dot > 0.999f)
		{
			return Normalize(Lerp(q0, q1, t));
		}

		float theta = acosf(dot);
		float sintheta = sinf(theta);
		float scale0 = sinf(theta * (1.0f - t)) / sintheta;
		float scale1 = sinf(theta * t) / sintheta;

		// Perform the slerp
		return Quaternion
		(
			(q0.w * scale0) + (q1.w * scale1),
			(q0.x * scale0) + (q1.x * scale1),
			(q0.y * scale0) + (q1.y * scale1),
			(q0.z * scale0) + (q1.z * scale1)
		);
	}
}