#include "ccpch.h"
#include "Framebuffer.h"

#include "Cataclysm/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Cataclysm
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	CC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(spec);
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}