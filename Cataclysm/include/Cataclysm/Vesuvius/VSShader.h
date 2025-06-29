#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Cataclysm::Vesuvius
{
	class VSShader
	{
	public:
		virtual ~VSShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static CCRef<VSShader> Create(const std::string& filepath);
		static CCRef<VSShader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const CCRef<VSShader>& shader);
		void Add(const std::string& name, const CCRef<VSShader>& shader);
		CCRef<VSShader> Load(const std::string& filepath);
		CCRef<VSShader> Load(const std::string& name, const std::string& filepath);

		CCRef<VSShader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, CCRef<VSShader>> m_Shaders;
	};
}
