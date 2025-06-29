#pragma once

#include "Cataclysm/Vesuvius/VSFramebuffer.h"

namespace Cataclysm::Vesuvius
{
	class VSOpenGLFramebuffer : public VSFramebuffer
	{
	public:
		VSOpenGLFramebuffer(const VSFramebufferSpecification& spec);
		virtual ~VSOpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { CC_CORE_ASSERT("Failed to get color attachment renderer ID!", index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }

		virtual const VSFramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		VSFramebufferSpecification m_Specification;

		std::vector<VSFramebufferTextureSpecification> m_ColorAttachmentSpecs;
		VSFramebufferTextureSpecification m_DepthAttachmentSpec = VSFramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
