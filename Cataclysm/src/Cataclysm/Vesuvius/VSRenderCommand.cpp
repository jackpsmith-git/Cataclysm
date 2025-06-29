#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSRenderCommand.h"

#include "Platform/OpenGL/VSOpenGLRendererAPI.h"

namespace Cataclysm::Vesuvius
{
	CCScope<VSRendererAPI> VSRenderCommand::s_RendererAPI = VSRendererAPI::Create();
}
