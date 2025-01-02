#pragma once

#include "Cataclysm/Renderer/VertexArray.h"

namespace Cataclysm
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Cataclysm::Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Cataclysm::Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Cataclysm::Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Cataclysm::Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Cataclysm::Ref<VertexBuffer>> m_VertexBuffers;
		Cataclysm::Ref<IndexBuffer> m_IndexBuffer;
	};
}