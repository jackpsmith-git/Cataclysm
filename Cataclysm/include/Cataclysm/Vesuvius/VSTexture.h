#pragma once

#include "Cataclysm/Core/CCCore.h"

#include <string>

namespace Cataclysm::Vesuvius
{
	enum class VSImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct VSTextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		VSImageFormat Format = VSImageFormat::RGBA8;
		bool GenerateMips = true;
	};

	class VSTexture
	{
	public:
		virtual ~VSTexture() = default;

		virtual const VSTextureSpecification& GetSpecification() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const VSTexture& other) const = 0;
	};

	class VSTexture2D : public VSTexture
	{
	public:
		static CCRef<VSTexture2D> Create(const VSTextureSpecification& specification);
		static CCRef<VSTexture2D> Create(const std::string& path);
		static CCRef<VSTexture2D> Create(const void* data, size_t size);
	};
}
