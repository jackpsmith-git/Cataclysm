#pragma once

namespace Cataclysm::Vesuvius
{
	enum class VSShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(VSShaderDataType type)
	{
		switch (type)
		{
			case VSShaderDataType::Float:		return 4;
			case VSShaderDataType::Float2:		return 4 * 2;
			case VSShaderDataType::Float3:		return 4 * 3;
			case VSShaderDataType::Float4:		return 4 * 4;
			case VSShaderDataType::Mat3:		return 4 * 3 * 3;
			case VSShaderDataType::Mat4:		return 4 * 4 * 4;
			case VSShaderDataType::Int:			return 4;
			case VSShaderDataType::Int2:		return 4 * 2;
			case VSShaderDataType::Int3:		return 4 * 3;
			case VSShaderDataType::Int4:		return 4 * 4;
			case VSShaderDataType::Bool:		return 1;
		}

		CC_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct VSBufferElement
	{
		std::string Name;
		VSShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		VSBufferElement() = default;

		VSBufferElement(VSShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case VSShaderDataType::Float:		return 1;
				case VSShaderDataType::Float2:		return 2;
				case VSShaderDataType::Float3:		return 3;
				case VSShaderDataType::Float4:		return 4;
				case VSShaderDataType::Mat3:		return 3 * 3;
				case VSShaderDataType::Mat4:		return 4 * 4;
				case VSShaderDataType::Int:			return 1;
				case VSShaderDataType::Int2:		return 2;
				case VSShaderDataType::Int3:		return 3;
				case VSShaderDataType::Int4:		return 4;
				case VSShaderDataType::Bool:		return 1;
			}

			CC_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class VSBufferLayout
	{
	public:
		VSBufferLayout() {}

		VSBufferLayout(const std::initializer_list<VSBufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<VSBufferElement>& GetElements() const { return m_Elements; }

		std::vector<VSBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<VSBufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<VSBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<VSBufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<VSBufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VSVertexBuffer
	{
	public:
		virtual ~VSVertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const VSBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VSBufferLayout& layout) = 0;

		static Cataclysm::CCRef<VSVertexBuffer> Create(uint32_t size);
		static Cataclysm::CCRef<VSVertexBuffer> Create(float* vertices, uint32_t size);
	};

	// Cataclysm currently only supports 32-bit index buffers
	class VSIndexBuffer
	{
	public:
		virtual ~VSIndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Cataclysm::CCRef<VSIndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}
