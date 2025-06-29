#pragma once

#include "Cataclysm/Core/CCLayer.h"

#include "Cataclysm/Events/CCApplicationEvent.h"
#include "Cataclysm/Events/CCKeyEvent.h"
#include "Cataclysm/Events/CCMouseEvent.h"

namespace Cataclysm
{
	class CCImGuiLayer : public CCLayer
	{
	public:
		CCImGuiLayer();
		~CCImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(CCEvent& event) override;

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
