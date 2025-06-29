#pragma once

#include "CPResources.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <Cataclysm.h>

namespace Cataclysm::CPImGui
{
	void BeginPanel(const char* name);
	void EndPanel();
	void PanelBackground();
	void BeginPanelMenu();
	void EndPanelMenu();
	void BeginPanelContent();
	void EndPanelContent();
}
