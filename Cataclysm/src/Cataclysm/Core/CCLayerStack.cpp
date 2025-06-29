#include "ccpch.h"
#include "Cataclysm/Core/CCLayerStack.h"

namespace Cataclysm
{
	CCLayerStack::CCLayerStack()
	{

	}

	CCLayerStack::~CCLayerStack()
	{
		for (CCLayer* layer : m_Layers)
			delete layer;
	}

	void CCLayerStack::PushLayer(CCLayer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void CCLayerStack::PushOverlay(CCLayer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void CCLayerStack::PopLayer(CCLayer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void CCLayerStack::PopOverlay(CCLayer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}
