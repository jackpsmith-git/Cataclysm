#pragma once

#include "Cataclysm/Vesuvius/VSTexture.h"

#include <glad/glad.h>

namespace Cataclysm::Vesuvius
{
	class VSOpenGLTexture2D : public VSTexture2D
	{
	public:
		VSOpenGLTexture2D(const VSTextureSpecification& specification);
		VSOpenGLTexture2D(const std::string& path);
		VSOpenGLTexture2D(const void* data, size_t size);
		virtual ~VSOpenGLTexture2D();

		virtual const VSTextureSpecification& GetSpecification() const override { return m_Specification; }

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const std::string& GetPath() const override 
		{
			return m_Path; 
		}

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const VSTexture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		VSTextureSpecification m_Specification;

		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}
