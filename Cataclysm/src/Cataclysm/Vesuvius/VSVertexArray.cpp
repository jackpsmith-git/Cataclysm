#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSVertexArray.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Platform/OpenGL/VSOpenGLVertexArray.h"

namespace Cataclysm::Vesuvius
{
	CCRef<VSVertexArray> VSVertexArray::Create()
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:		CC_CORE_ASSERT(false, "VSRendererAPI::None is currently non supported!");  return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:		return CreateRef<VSOpenGLVertexArray>();
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
