#pragma once

#include "Cataclysm.h"

#include "ParticleSystem.h"

class Sandbox2D : public Cataclysm::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Cataclysm::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Cataclysm::Event& e) override;

private:
	Cataclysm::OrthographicCameraController m_CameraController;

	Cataclysm::Ref<Cataclysm::VertexArray> m_SquareVA;
	Cataclysm::Ref<Cataclysm::Shader> m_FlatColorShader;

	Cataclysm::Ref<Cataclysm::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};