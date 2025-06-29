#pragma once

#include "Cataclysm.h"
#include <filesystem>

namespace Cataclysm
{
	class GameLayer : public Cataclysm::CCLayer 
	{
	public:
		GameLayer();
		virtual ~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		bool OpenGame();
		void OpenGame(const std::filesystem::path& path);

		void OnUpdate(Cataclysm::CCTimestep ts) override;
		void OnImGuiRender(Cataclysm::CCTimestep ts) override;
	private:
		Cataclysm::CCRef<Cataclysm::CCScene> m_Scene;
		Cataclysm::CCRef<Vesuvius::VSFramebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}
