#pragma once

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCLayer.h"

#include <vector>

namespace Cataclysm
{
	class CCLayerStack
	{
	public:
		CCLayerStack();
		~CCLayerStack();

		void PushLayer(CCLayer* layer);
		void PushOverlay(CCLayer* overlay);
		void PopLayer(CCLayer* layer);
		void PopOverlay(CCLayer* overlay);

		std::vector<CCLayer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<CCLayer*>::iterator end() { return m_Layers.end(); }

		std::vector<CCLayer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<CCLayer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<CCLayer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<CCLayer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<CCLayer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<CCLayer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<CCLayer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
