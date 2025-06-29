#pragma once

#include "Cataclysm/Vesuvius/VSRendererAPI.h"

namespace Cataclysm::Vesuvius
{
	class VSOpenGLRendererAPI : public VSRendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const CCRef<VSVertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const CCRef<VSVertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void SetLineWidth(float width) override;
	};
}
