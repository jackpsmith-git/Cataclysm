#pragma once

#include "CCProject.h"

namespace Cataclysm 
{
	class CCProjectSerializer
	{
	public:
		CCProjectSerializer(CCRef<CCProject> project);
		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		CCRef<CCProject> m_Project;
	};
}
