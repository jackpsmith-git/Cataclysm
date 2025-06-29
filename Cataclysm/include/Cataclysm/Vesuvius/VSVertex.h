#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace Cataclysm::Vesuvius
{
	struct VSQuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TextureIndex;
		float TilingFactor;

		int EntityID;
	};

	struct VSCircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		int EntityID;
	};

	struct VSLineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int EntityID;
	};

	struct VSTextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		// TODO: Color for outline/background

		int EntityID;
	};
}
