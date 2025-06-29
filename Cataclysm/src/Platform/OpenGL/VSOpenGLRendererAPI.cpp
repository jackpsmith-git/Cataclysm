#include "ccpch.h"
#include "Platform/OpenGL/VSOpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Cataclysm::Vesuvius
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         CC_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       CC_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          CC_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: CC_CORE_TRACE(message); return;
		}

		CC_CORE_ASSERT(false, "Unknown severity level!");
	}

	void VSOpenGLRendererAPI::Init()
	{
#ifdef CC_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void VSOpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void VSOpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void VSOpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void VSOpenGLRendererAPI::DrawIndexed(const CCRef<VSVertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void VSOpenGLRendererAPI::DrawLines(const CCRef<VSVertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void VSOpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}
