#pragma once

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCTimestep.h"

#include "Cataclysm/Events/CCEvent.h"

namespace Cataclysm
{
	class CCLayer
	{
	public:
		CCLayer(const std::string& name = "Layer");
		virtual ~CCLayer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(CCTimestep ts) {}
		virtual void OnImGuiRender(CCTimestep ts) {}
		virtual void OnEvent(CCEvent& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
