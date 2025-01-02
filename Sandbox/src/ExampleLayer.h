#pragma once

#include "Cataclysm.h"

class ExampleLayer : public Cataclysm::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Cataclysm::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Cataclysm::Event& e) override;
private:
	Cataclysm::ShaderLibrary m_ShaderLibrary;
	Cataclysm::Ref<Cataclysm::Shader> m_Shader;
	Cataclysm::Ref<Cataclysm::VertexArray> m_VertexArray;
	Cataclysm::Ref<Cataclysm::Shader> m_FlatColorShader;
	Cataclysm::Ref<Cataclysm::VertexArray> m_SquareVA;
	Cataclysm::Ref<Cataclysm::Texture2D> m_Texture, m_ChernoLogoTexture;
	Cataclysm::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};