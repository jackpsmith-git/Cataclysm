#pragma once

#include "Cataclysm/Core/UUID.h"

#include <box2d/b2_world_callbacks.h>
#include <vector>

namespace Cataclysm {

	class ContactListener2D : public b2ContactListener
	{
	public:
		struct CollisionInfo
		{
			UUID EntityA, EntityB;
		};

		const std::vector<CollisionInfo>& GetCollisions() const { return m_Collisions; }
		const std::vector<CollisionInfo>& GetEndCollisions() const { return m_EndCollisions; }
		void Clear() { m_Collisions.clear(); }
		void ClearEndCollisions() { m_EndCollisions.clear(); }

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

	private:
		std::vector<CollisionInfo> m_Collisions;
		std::vector<CollisionInfo> m_EndCollisions;
	};

}
