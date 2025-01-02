#pragma once

#include "Cataclysm/Core/Layer.h"

#include "Cataclysm/Events/ApplicationEvent.h"
#include "Cataclysm/Events/KeyEvent.h"
#include "Cataclysm/Events/MouseEvent.h"

namespace Cataclysm
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}
