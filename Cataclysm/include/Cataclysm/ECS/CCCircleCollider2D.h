#pragma once

#include <glm/glm.hpp>

namespace Cataclysm
{
	struct CircleCollider2D
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.5f;
		float RestitutionThreshold = 0.5f;

		bool IsTrigger = false;

		void* RuntimeFixture = nullptr;

		CircleCollider2D() = default;
		CircleCollider2D(const CircleCollider2D&) = default;

		void Reset()
		{
			Offset = { 0.0f, 0.0f };
			Radius = 0.5f;
			Density = 1.0f;
			Friction = 0.5f;
			Restitution = 0.5f;
			RestitutionThreshold = 0.5f;
			IsTrigger = false;
		}
	};
}
