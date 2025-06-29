#pragma once

#include <glm/glm.hpp>

#include "VSVertexArray.h"

namespace Cataclysm::Vesuvius
{
	class VSRendererAPI
	{
	public:
		enum class VSAPI
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~VSRendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const CCRef<VSVertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const CCRef<VSVertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		inline static VSAPI GetAPI() { return s_API; }
		static CCScope<VSRendererAPI> Create();
	private:
		static VSAPI s_API;
	};
}
