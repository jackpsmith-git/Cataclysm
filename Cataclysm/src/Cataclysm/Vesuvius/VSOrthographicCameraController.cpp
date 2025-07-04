#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSOrthographicCameraController.h"

#include "Cataclysm/Input/CCInput.h"

namespace Cataclysm::Vesuvius
{
	VSOrthographicCameraController::VSOrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void VSOrthographicCameraController::OnUpdate(CCTimestep ts)
	{
		if (CCInput::IsKeyDown(CC_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (CCInput::IsKeyDown(CC_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (CCInput::IsKeyDown(CC_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (CCInput::IsKeyDown(CC_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (CCInput::IsKeyDown(CC_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (CCInput::IsKeyDown(CC_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void VSOrthographicCameraController::OnEvent(CCEvent& e)
	{
		CCEventDispatcher dispatcher(e);
		dispatcher.Dispatch<CCMouseScrolledEvent>(CC_BIND_EVENT_FN(VSOrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<CCWindowResizeEvent>(CC_BIND_EVENT_FN(VSOrthographicCameraController::OnWindowResized));
	}

	void VSOrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool VSOrthographicCameraController::OnMouseScrolled(CCMouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool VSOrthographicCameraController::OnWindowResized(CCWindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}
