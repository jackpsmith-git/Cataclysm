#pragma once

#include "VSCamera.h"
#include "Cataclysm/Core/CCTimestep.h"
#include "Cataclysm/Events/CCEvent.h"
#include "Cataclysm/Events/CCMouseEvent.h"

#include <glm/glm.hpp>

namespace Cataclysm::Vesuvius
{
	class VSEditorCamera : public VSCamera
	{
	public:
		VSEditorCamera() = default;
		VSEditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(CCTimestep ts);
		void OnEvent(CCEvent& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		void Reset() {
			m_FOV = 45.0f;
			m_AspectRatio = 1.778f;
			m_NearClip = 0.1f;
			m_FarClip = 1000.0f;
			m_Position = { 0.0f, 0.0f, -10.0f };
			m_FocalPoint = { 0.0f, 0.0f, 0.0f };
			m_Distance = 10.0f;
			m_Pitch = 0.0f;
			m_Yaw = 0.0f;
			UpdateProjection();
			UpdateView();
		}

		void CopyValues(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 position, glm::vec3 focalPoint, float distance, float pitch, float yaw, float viewportWidth, float viewportHeight) {
			m_FOV = fov;
			m_AspectRatio = aspectRatio;
			m_NearClip = nearClip;
			m_FarClip = farClip;
			m_Position = position;
			m_FocalPoint = focalPoint;
			m_Distance = distance;
			m_Pitch = pitch;
			m_Yaw = yaw;
			m_ViewportWidth = viewportWidth;
			m_ViewportHeight = viewportHeight;
			UpdateProjection();
			UpdateView();
		}
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(CCMouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, -10.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition;

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}
