#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSRendererAPI.h"

#include "Platform/OpenGL/VSOpenGLRendererAPI.h"

namespace Cataclysm::Vesuvius
{
	VSRendererAPI::VSAPI VSRendererAPI::s_API = VSRendererAPI::VSAPI::OpenGL;

	CCScope<VSRendererAPI> VSRendererAPI::Create()
	{
		switch (s_API)
		{
			case VSRendererAPI::VSAPI::None:    CC_CORE_ASSERT(false, "VSRendererAPI::None is currently not supported!"); return nullptr;
			case VSRendererAPI::VSAPI::OpenGL:  return CreateScope<VSOpenGLRendererAPI>();
		}

		CC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
