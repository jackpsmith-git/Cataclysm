#pragma once

#include "CPResources.h"
#include "ImGui/CPImGuiPanel.h"

namespace Cataclysm::CPImGui
{
	bool ColorSelect(const char* label, float* color, float columnWidth = 100.0f);
	bool InputTextMultiline(const char* label, std::string* string, float columnWidth = 100.0f);
	bool InputDouble(const char* label, double* value, float columnWidth = 100.0f);

	bool BeginCombo(const char* label, const char* value, float columnWidth = 100.0f);

	bool TreeNode(std::string& id, CCRef<Vesuvius::VSTexture2D> icon, ImGuiTreeNodeFlags flags, const char* label);
	void EntityNode(Entity entity, CCRef<CCScene> context, Entity& selectionContext);

	bool DragVec2(const std::string& label, const std::string& uniqueID, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	bool DragVec3(const std::string& label, const std::string& uniqueID, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	bool DragVec4(const std::string& label, const std::string& uniqueID, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	bool SliderFloat(const char* label, float* value, float min, float max, float width = 150.0f);

	bool FramedButton(const char* label, const char* buttonLabel, float columnWidth = 100.0f);
	bool FramedButtonWithIcon(const char* label, CCRef<Vesuvius::VSTexture2D> icon, const char* buttonLabel, float columnWidth = 100.0f);
	bool FramedCheckbox(const char* label, bool* value, float columnWidth = 100.0f);
	bool FramedDragFloat(const char* label, float* value, float speed = 0.01f, float min = -FLT_MAX, float max = FLT_MAX, const char* format = "%.3f", float columnWidth = 100.0f);
	bool FramedDragInt(const char* label, int* value, float speed = 0.01f, float min = -INT_MAX, float max = FLT_MAX, float columnWidth = 100.0f);
	bool TagCheckbox(bool* value);

	bool ImageButton(CCRef<Vesuvius::VSTexture2D> icon, float size, ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	bool ImageButton(CCRef<Vesuvius::VSTexture2D> icon, ImVec2& size, ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	void Image(CCRef<Vesuvius::VSTexture2D> icon, float size, ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	void Image(CCRef<Vesuvius::VSTexture2D> icon, ImVec2& size, ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	void Image(uint32_t textureID, ImVec2& size, ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	bool MenuItem(const char* label, CCRef<Vesuvius::VSTexture2D> icon = nullptr, const char* shortcut = nullptr, bool enabled = true, bool selected = false);
};
