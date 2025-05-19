#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Renderer/Texture.h"

namespace Cataclysm
{
	class GamePanel
	{
	public:
		GamePanel();

		void OnImGuiRender(const Ref<Scene>& scene, Timestep ts, int state);
		void Init();
	private:
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		Ref<Framebuffer> m_Framebuffer;
	};
}
