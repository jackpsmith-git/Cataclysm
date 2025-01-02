#include "ccpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Cataclysm
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		CC_CORE_ASSERT(false, "RendererAPI::None is currently non supported!");  return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexArray>();
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}