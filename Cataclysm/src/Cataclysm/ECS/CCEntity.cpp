#include "ccpch.h"
#include "Cataclysm/ECS/CCEntity.h"

namespace Cataclysm
{
	Entity::Entity(entt::entity handle, CCScene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}
