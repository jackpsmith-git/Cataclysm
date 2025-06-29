#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSTexture.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Platform/OpenGL/VSOpenGLTexture.h"

namespace Cataclysm::Vesuvius
{
	CCRef<VSTexture2D> VSTexture2D::Create(const VSTextureSpecification& specification)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:		CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:		return CreateRef<VSOpenGLTexture2D>(specification);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	CCRef<VSTexture2D> VSTexture2D::Create(const std::string& path)
	{

		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:		CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:		return CreateRef<VSOpenGLTexture2D>(path);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	CCRef<VSTexture2D> VSTexture2D::Create(const void* data, size_t size)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:		CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:		return CreateRef<VSOpenGLTexture2D>(data, size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
