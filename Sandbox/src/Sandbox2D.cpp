#include "ccpch.h"
#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f})
{

}

void Sandbox2D::OnAttach()
{
	CC_PROFILE_FUNCTION();

	m_CheckerboardTexture = Cataclysm::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	CC_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Cataclysm::Timestep ts)
{
	CC_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Cataclysm::Vesuvius::ResetStats();
	{
		CC_PROFILE_SCOPE("Renderer Prep");
		Cataclysm::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Cataclysm::RenderCommand::Clear();
	}

	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	Cataclysm::Vesuvius::BeginScene(m_CameraController.GetCamera());
	Cataclysm::Vesuvius::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
	Cataclysm::Vesuvius::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Cataclysm::Vesuvius::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Cataclysm::Vesuvius::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
	Cataclysm::Vesuvius::DrawQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
	Cataclysm::Vesuvius::EndScene();

	Cataclysm::Vesuvius::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
			Cataclysm::Vesuvius::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}
	Cataclysm::Vesuvius::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	CC_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Cataclysm::Vesuvius::GetStats();
	ImGui::Text("Renderer 2D Stats: ");
	ImGui::Text("Draw Calls:	%d", stats.DrawCalls);
	ImGui::Text("Quads:			%d", stats.QuadCount);
	ImGui::Text("Vertices:		%d", stats.GetTotalVertexCount());
	ImGui::Text("Indices:		%d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Cataclysm::Event& e)
{
	m_CameraController.OnEvent(e);
}