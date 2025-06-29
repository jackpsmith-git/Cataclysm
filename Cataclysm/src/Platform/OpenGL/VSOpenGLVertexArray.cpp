#include "ccpch.h"
#include "Platform/OpenGL/VSOpenGLVertexArray.h"

#include <glad/glad.h>

namespace Cataclysm::Vesuvius
{
	static GLenum ShaderDataTypeToOpenGLBaseType(VSShaderDataType type)
	{
		switch (type)
		{
			case VSShaderDataType::None:		return GL_FLOAT;
			case VSShaderDataType::Float:		return GL_FLOAT;
			case VSShaderDataType::Float2:		return GL_FLOAT;
			case VSShaderDataType::Float3:		return GL_FLOAT;
			case VSShaderDataType::Float4:		return GL_FLOAT;
			case VSShaderDataType::Mat3:		return GL_FLOAT;
			case VSShaderDataType::Mat4:		return GL_FLOAT;
			case VSShaderDataType::Int:			return GL_INT;
			case VSShaderDataType::Int2:		return GL_INT;
			case VSShaderDataType::Int3:		return GL_INT;
			case VSShaderDataType::Int4:		return GL_INT;
			case VSShaderDataType::Bool:		return GL_BOOL;
		}

		CC_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VSOpenGLVertexArray::VSOpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	VSOpenGLVertexArray::~VSOpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VSOpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VSOpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VSOpenGLVertexArray::AddVertexBuffer(const Cataclysm::CCRef<VSVertexBuffer>& vertexBuffer)
	{
		CC_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case VSShaderDataType::Float:
				case VSShaderDataType::Float2:
				case VSShaderDataType::Float3:
				case VSShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(
						m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					m_VertexBufferIndex++;
					break;

				}
				case VSShaderDataType::Int:
				case VSShaderDataType::Int2:
				case VSShaderDataType::Int3:
				case VSShaderDataType::Int4:
				case VSShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(
						m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);
					m_VertexBufferIndex++;
					break;
				}
				case VSShaderDataType::Mat3:
				case VSShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(m_VertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_VertexBufferIndex, 1);
						m_VertexBufferIndex++;
					}
					break;
				}
			}
		}
		
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VSOpenGLVertexArray::SetIndexBuffer(const Cataclysm::CCRef<VSIndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
