#pragma once

#include <glm/glm.hpp>

namespace Cataclysm::Vesuvius
{
	class VSCamera
	{
	public:
		VSCamera() = default;
		VSCamera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~VSCamera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}
