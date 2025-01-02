#include "ccpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Cataclysm
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	CC_CORE_ASSERT(false, "RendererAPI::None is currently non supported!");  return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	CC_CORE_ASSERT(false, "RendererAPI::None is currently non supported!");  return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	CC_CORE_ASSERT(false, "RendererAPI::None is currently non supported!");  return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}