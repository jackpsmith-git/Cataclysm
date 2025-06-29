#pragma once

#include "Cataclysm/Core/CCUUID.h"
#include "Cataclysm/ECS/CCEntity.h"

namespace Cataclysm
{
	class CCScenePhysicsManager
	{
	public:
		static void OnPhysics2DStart(CCScene* scene);
		static void OnPhysics2DStop(CCScene* scene);
		static void ProcessPhysics(CCScene* scene, float ts);
		static void ProcessCollisions(CCScene* scene);
	};
}
