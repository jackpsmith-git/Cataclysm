#include "ccpch.h"
#include "Cataclysm/Scene/CCSceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cataclysm
{
	CCSceneCamera::CCSceneCamera()
	{
		RecalculateProjection();
	}

	void CCSceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = CCProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void CCSceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
	{
		m_ProjectionType = CCProjectionType::Perspective;
		m_PerspectiveFOV = verticalFov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void CCSceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void CCSceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == CCProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(
				orthoLeft, 
				orthoRight, 
				orthoBottom, 
				orthoTop, 
				m_OrthographicNear, 
				m_OrthographicFar);
		}
	}
}
