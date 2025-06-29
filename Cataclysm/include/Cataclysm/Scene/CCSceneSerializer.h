#pragma once

#include "CCScene.h"

namespace Cataclysm
{
	class CCSceneSerializer
	{
	public:
		CCSceneSerializer(const CCRef<CCScene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		CCRef<CCScene> m_Scene;
	};
}
