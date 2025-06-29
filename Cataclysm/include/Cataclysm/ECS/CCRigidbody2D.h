#pragma once

namespace Cataclysm
{
	struct Rigidbody2D
	{
	public:
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Dynamic;
		bool FixedRotation = false;
		float GravityScale = 1.0f;
		float Mass = 1.0f;

		void* RuntimeBody = nullptr;

		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D&) = default;

		void Reset()
		{
			Type = BodyType::Dynamic;
			FixedRotation = false;
			GravityScale = 1.0f;
			Mass = 1.0f;
		}
	};
}
