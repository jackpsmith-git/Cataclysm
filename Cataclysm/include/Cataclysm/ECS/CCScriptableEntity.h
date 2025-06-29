#pragma once

#include "Cataclysm/ECS/CCEntity.h"

namespace Cataclysm
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(CCTimestep ts) {}

	private:
		Entity m_Entity;
		friend class CCScene;
	};
}
