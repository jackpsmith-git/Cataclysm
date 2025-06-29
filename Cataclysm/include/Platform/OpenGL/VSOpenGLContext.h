#pragma once

#include "Cataclysm/Vesuvius/VSGraphicsContext.h"

struct GLFWwindow;

namespace Cataclysm::Vesuvius
{
	class VSOpenGLContext : public VSGraphicsContext
	{
	public:
		VSOpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
