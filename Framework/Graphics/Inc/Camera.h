#pragma once

namespace SAVAGE::Graphics
{
	// This camera assumes YAxis as up direction. You
	// cannot look straight up nor straight down.
	class Camera
	{
	public:
		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);
		void SetLookAt(const Math::Vector3& target);

		void SetFov(float fov);
		void SetAspectRatio(float ratio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		// 3 degrees of freedom for translations
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);

		// 2 degrees of freedom for rotations
		void Yaw(float radian);
		void Pitch(float radian);

		// 1 degree of freedom for field of view
		void Zoom(float amount);

		const Math::Vector3& GetPosition() const;
		const Math::Vector3& GetDirection() const;

		Math::Matrix4 GetViewMatrix() const;
		Math::Matrix4 GetProjectionMatrix() const;

	private:
		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		float mFov = 60.0f * Math::Constants::DegToRad;
		float mAspectRatio = 0.0f;
		float mNearPlane = 1.0f;
		float mFarPlane = 10000.0f;
	};
}
