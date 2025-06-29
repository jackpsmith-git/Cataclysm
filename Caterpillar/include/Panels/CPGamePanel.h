#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Vesuvius/VSTexture.h"

namespace Cataclysm
{
	class CPGamePanel
	{
	public:
		CPGamePanel();

		void OnImGuiRender(const CCRef<CCScene>& scene, CCTimestep ts, int state);
		void Init();
	private:
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		CCRef<Vesuvius::VSFramebuffer> m_Framebuffer;
	};
}
