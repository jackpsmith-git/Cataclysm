#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSGraphicsContext.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Platform/OpenGL/VSOpenGLContext.h"

namespace Cataclysm::Vesuvius
{
	CCScope<VSGraphicsContext> VSGraphicsContext::Create(void* window)
	{
		switch (VSRenderer::GetAPI())
		{
			case VSRendererAPI::VSAPI::None:    CC_CORE_ASSERT(false, "VSRendererAPI::None is currently not supported!"); return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:  return CreateScope<VSOpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
