#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSBuffer.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Platform/OpenGL/VSOpenGLBuffer.h"

namespace Cataclysm::Vesuvius
{
	Cataclysm::CCRef<VSVertexBuffer> VSVertexBuffer::Create(uint32_t size)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:	CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:	return Cataclysm::CreateRef<VSOpenGLVertexBuffer>(size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Cataclysm::CCRef<VSVertexBuffer> VSVertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:	CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:	return Cataclysm::CreateRef<VSOpenGLVertexBuffer>(vertices, size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Cataclysm::CCRef<VSIndexBuffer> VSIndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:	CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:	return Cataclysm::CreateRef<VSOpenGLIndexBuffer>(indices, size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
