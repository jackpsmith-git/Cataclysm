#pragma once

#include "Cataclysm/Core/CCUUID.h"

#include <box2d/b2_world_callbacks.h>
#include <vector>

namespace Cataclysm {

	class CCContactListener2D : public b2ContactListener
	{
	public:
		struct CCCollisionInfo
		{
			CCUUID EntityA, EntityB;
		};

		const std::vector<CCCollisionInfo>& GetCollisions() const { return m_Collisions; }
		const std::vector<CCCollisionInfo>& GetEndCollisions() const { return m_EndCollisions; }
		void Clear() { m_Collisions.clear(); }
		void ClearEndCollisions() { m_EndCollisions.clear(); }

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

	private:
		std::vector<CCCollisionInfo> m_Collisions;
		std::vector<CCCollisionInfo> m_EndCollisions;
	};

}
