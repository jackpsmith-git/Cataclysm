#include "ccpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Cataclysm
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}