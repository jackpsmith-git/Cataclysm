#pragma once

#include "Cataclysm/Core/CCCore.h"

namespace Cataclysm::Vesuvius
{
	enum class VSFramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct VSFramebufferTextureSpecification
	{
		VSFramebufferTextureSpecification() = default;
		VSFramebufferTextureSpecification(VSFramebufferTextureFormat format)
			: TextureFormat(format) {}

		VSFramebufferTextureFormat TextureFormat = VSFramebufferTextureFormat::None;
	};

	struct VSFramebufferAttachmentSpecification
	{
		VSFramebufferAttachmentSpecification() = default;
		VSFramebufferAttachmentSpecification(std::initializer_list<VSFramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<VSFramebufferTextureSpecification> Attachments;
	};

	struct VSFramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		VSFramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class VSFramebuffer
	{
	public:
		virtual ~VSFramebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const VSFramebufferSpecification& GetSpecification() const = 0;

		static CCRef<VSFramebuffer> Create(const VSFramebufferSpecification& spec);
	};
}
