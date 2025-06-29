#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSUniformBuffer.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Platform/OpenGL/VSOpenGLUniformBuffer.h"

namespace Cataclysm::Vesuvius
{
	CCRef<VSUniformBuffer> VSUniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:    CC_CORE_ASSERT(false, "VSRendererAPI::None is currently not supported!"); return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:  return CreateRef<VSOpenGLUniformBuffer>(size, binding);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
