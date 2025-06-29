#pragma once

#include "Cataclysm/Vesuvius/VSOrthographicCamera.h"
#include "Cataclysm/Core/CCTimestep.h"
#include "Cataclysm/Events/CCApplicationEvent.h"
#include "Cataclysm/Events/CCMouseEvent.h"

namespace Cataclysm::Vesuvius
{
	struct VSOrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class VSOrthographicCameraController
	{
	public:
		VSOrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(CCTimestep ts);
		void OnEvent(CCEvent& e);

		void OnResize(float width, float height);

		VSOrthographicCamera& GetCamera() { return m_Camera; }
		const VSOrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { 
			m_ZoomLevel = level;
		}

		const VSOrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		bool OnMouseScrolled(CCMouseScrolledEvent& e);
		bool OnWindowResized(CCWindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		VSOrthographicCamera m_Camera;
		VSOrthographicCameraBounds m_Bounds;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};
}
