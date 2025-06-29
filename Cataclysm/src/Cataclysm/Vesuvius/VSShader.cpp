#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSShader.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Platform/OpenGL/VSOpenGLShader.h"

namespace Cataclysm::Vesuvius
{
	CCRef<VSShader> VSShader::Create(const std::string& filepath)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:	CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:	return CreateRef<VSOpenGLShader>(filepath);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	CCRef<VSShader> VSShader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:	CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:	return CreateRef<VSOpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const CCRef<VSShader>& shader)
	{
		CC_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const CCRef<VSShader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	CCRef<VSShader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = VSShader::Create(filepath);
		Add(shader);
		return shader;
	}

	CCRef<VSShader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = VSShader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	CCRef<VSShader> ShaderLibrary::Get(const std::string& name)
	{
		CC_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
