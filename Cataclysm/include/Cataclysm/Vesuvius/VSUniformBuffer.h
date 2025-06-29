#pragma once

#include "Cataclysm/Core/CCCore.h"

namespace Cataclysm::Vesuvius
{
	class VSUniformBuffer
	{
	public:
		virtual ~VSUniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static CCRef<VSUniformBuffer> Create(uint32_t size, uint32_t binding);
	};
}
