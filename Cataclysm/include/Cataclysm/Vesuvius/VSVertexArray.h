#pragma once

#include <memory>
#include "Cataclysm/Vesuvius/VSBuffer.h"

namespace Cataclysm::Vesuvius
{
	class VSVertexArray
	{
	public:
		virtual ~VSVertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const CCRef<VSVertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const CCRef<VSIndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<CCRef<VSVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const CCRef<VSIndexBuffer>& GetIndexBuffer() const = 0;

		static CCRef<VSVertexArray> Create();
	};
}
