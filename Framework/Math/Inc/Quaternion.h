#pragma once

namespace SAVAGE::Math
{
	struct Matrix4;

	struct Quaternion
	{
		float w, x, y, z;

		constexpr Quaternion() noexcept : w{ 1.0f }, x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}
		constexpr Quaternion(float w, float x, float y, float z) noexcept : w{ w }, x{ x }, y{ y }, z{ z } {}

		// Commonly used Quaternions
		const static Quaternion Zero;
		const static Quaternion Identity;

		static Quaternion RotationAxis(const Vector3& axis, float rad);
		static Quaternion RotationMatrix(const Matrix4& m);
		static Quaternion RotationLook(const Vector3& direction, const Vector3& up = Vector3::YAxis);
		static Quaternion RotationFromTo(const Vector3& from, const Vector3& to);

		constexpr Quaternion operator-() const { return { -w, -x, -y, -z }; }
		constexpr Quaternion operator+(const Quaternion& q) const { return { w + q.w, x + q.x, y + q.y, z + q.z }; }
		constexpr Quaternion operator-(const Quaternion& q) const { return { w - q.w, x - q.x, y - q.y, z - q.z }; }
		constexpr Quaternion operator*(const Quaternion& q) const
		{
			return
			{
				(q.w * w) - (q.x * x) - (q.y * y) - (q.z * z),
				(q.w * x) + (q.x * w) - (q.y * z) + (q.z * y),
				(q.w * y) + (q.x * z) + (q.y * w) - (q.z * x),
				(q.w * z) - (q.x * y) + (q.y * x) + (q.z * w)
			};
		}

		constexpr Quaternion operator*(float s) const { return { w * s, x * s, y * s, z * s }; }
		constexpr Quaternion operator/(float s) const { return Quaternion(w / s, x / s, y / s, z / s); }

		//Quaternion& operator+=(const Quaternion& q) { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
		//Quaternion& operator-=(const Quaternion& q) { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
		//Quaternion& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
		//Quaternion& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

	};
}