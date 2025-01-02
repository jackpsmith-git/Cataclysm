#include "ccpch.h"
#include "Cataclysm/Renderer/GraphicsContext.h"

#include "Cataclysm/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Cataclysm {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}