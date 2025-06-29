#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSFramebuffer.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"

#include "Platform/OpenGL/VSOpenGLFramebuffer.h"

namespace Cataclysm::Vesuvius
{
	CCRef<VSFramebuffer> VSFramebuffer::Create(const VSFramebufferSpecification& spec)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:	CC_CORE_ASSERT(false, "VSRendererAPI::None is currently not supported!"); return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:	return CreateRef<VSOpenGLFramebuffer>(spec);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
