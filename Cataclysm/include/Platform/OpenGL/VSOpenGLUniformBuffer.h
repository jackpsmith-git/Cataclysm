#pragma once

#include "Cataclysm/Vesuvius/VSUniformBuffer.h"

namespace Cataclysm::Vesuvius
{
	class VSOpenGLUniformBuffer : public VSUniformBuffer
	{
	public:
		VSOpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~VSOpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_RendererID = 0;
	};
}
