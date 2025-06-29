#pragma once

#include "Cataclysm/Vesuvius/VSBuffer.h"

namespace Cataclysm::Vesuvius
{
	class VSOpenGLVertexBuffer : public VSVertexBuffer
	{
	public:
		VSOpenGLVertexBuffer(uint32_t size);
		VSOpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~VSOpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const VSBufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VSBufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		VSBufferLayout m_Layout;
	};

	class VSOpenGLIndexBuffer : public VSIndexBuffer
	{
	public:
		VSOpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~VSOpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}
