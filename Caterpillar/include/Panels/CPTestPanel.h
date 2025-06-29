#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class CPTestPanel
	{
	public:
		CPTestPanel();

		void OnImGuiRender();
	private:
		float m_SliderValue = 1.0f;
	};
}
