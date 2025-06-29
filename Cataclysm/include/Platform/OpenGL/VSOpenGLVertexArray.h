#pragma once

#include "Cataclysm/Vesuvius/VSVertexArray.h"

namespace Cataclysm::Vesuvius
{
	class VSOpenGLVertexArray : public VSVertexArray
	{
	public:
		VSOpenGLVertexArray();
		virtual ~VSOpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Cataclysm::CCRef<VSVertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Cataclysm::CCRef<VSIndexBuffer>& indexBuffer) override;

		virtual const std::vector<Cataclysm::CCRef<VSVertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Cataclysm::CCRef<VSIndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Cataclysm::CCRef<VSVertexBuffer>> m_VertexBuffers;
		Cataclysm::CCRef<VSIndexBuffer> m_IndexBuffer;
	};
}
