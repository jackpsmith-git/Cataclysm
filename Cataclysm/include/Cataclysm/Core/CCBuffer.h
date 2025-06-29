#pragma once

#include <stdint.h>
#include <cstring>

namespace Cataclysm
{
	struct CCBuffer
	{
		uint8_t* Data = nullptr;
		uint64_t Size = 0;

		CCBuffer() = default;

		CCBuffer(uint64_t size)
		{
			Allocate(size);
		}

		CCBuffer(const CCBuffer&) = default;

		static CCBuffer Copy(CCBuffer other)
		{
			CCBuffer result(other.Size);
			memcpy(result.Data, other.Data, other.Size);
			return result;
		}

		void Allocate(uint64_t size)
		{
			Release();

			Data = new uint8_t[size];
			Size = size;
		}

		void Release()
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		operator bool() const
		{
			return (bool)Data;
		}
	};

	struct CCScopedBuffer
	{
		CCScopedBuffer(CCBuffer buffer)
			: m_Buffer(buffer)
		{
		}

		CCScopedBuffer(uint64_t size)
			: m_Buffer(size)
		{
		}

		~CCScopedBuffer()
		{
			m_Buffer.Release();
		}

		uint8_t* Data() { return m_Buffer.Data; }
		uint64_t Size() { return m_Buffer.Size; }

		template<typename T>
		T* As()
		{
			return m_Buffer.As<T>();
		}

		operator bool() const { return m_Buffer; }
	private:
		CCBuffer m_Buffer;
	};
}
