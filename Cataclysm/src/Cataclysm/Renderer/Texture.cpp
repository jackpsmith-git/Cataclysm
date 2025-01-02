#include "ccpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Cataclysm
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		CC_CORE_ASSERT(false, "RendererAPI::None is currently non supported!");  return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(specification);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		CC_CORE_ASSERT(false, "RendererAPI::None is currently non supported!");  return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
