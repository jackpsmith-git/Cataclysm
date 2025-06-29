#pragma once

#include "Cataclysm/Core/CCUUID.h"
#include <Cataclysm/ECS/CCEntity.h>

namespace Cataclysm
{
	class CCSceneEntityManager
	{
	public:
		static Entity CreateEntity(CCScene* scene, const std::string& name = std::string());
		static Entity CreateEntityWithUUID(CCScene* scene, CCUUID uuid, const std::string& name = std::string());
		static void DestroyEntity(CCScene* scene, Entity entity);
		static Entity GetEntityByUUID(CCScene* scene, CCUUID uuid);
		static Entity FindEntityByName(CCScene* scene, std::string_view name);
		static Entity DuplicateEntity(CCScene* scene, Entity entity);

		static bool IsEnabled(CCScene* scene, Entity entity);

		static CCUUID GetParent(CCScene* scene, CCUUID entity);
		static void SetParent(CCScene* scene, CCUUID entity, CCUUID parent);
		static void SetParentOnLoad(CCScene* scene, CCUUID entity, CCUUID parent);
		static void RemoveParent(CCScene* scene, CCUUID entity);
		static std::vector<CCUUID> GetChildren(CCScene* scene, CCUUID entity);

		static glm::mat4 GetWorldTransform(CCScene* scene, CCUUID entityID);
		static void SetWorldTransform(CCScene* scene, CCUUID entityID, const glm::mat4& worldTransform);

		static Entity GetPrimaryCameraEntity(CCScene* scene);

		static void UpdateTransforms(CCScene* scene);
		static glm::mat4 ComposeTransform(CCScene* scene, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
		static void UpdateTransformRecursive(CCScene* scene, Entity entity);

		template<typename... Components>
		static auto GetAllEntitiesWith(CCScene* scene)
		{
			return scene->GetRegistry().view<Components...>();
		}
	};
}
