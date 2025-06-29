#include "ccpch.h"
#include "Cataclysm/Project/CCProject.h"

#include "Cataclysm/Project/CCProjectSerializer.h"

namespace Cataclysm 
{
	CCRef<CCProject> CCProject::New()
	{
		s_ActiveProject = CreateRef<CCProject>();
		return s_ActiveProject;
	}

	CCRef<CCProject> CCProject::Load(const std::filesystem::path& path)
	{
		CCRef<CCProject> project = CreateRef<CCProject>();
		CCProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			std::string windowTitle = "Caterpillar - " + s_ActiveProject->GetConfig().Name;
			CCApplication::Get().GetWindow().SetWindowTitle(windowTitle.c_str());
			return s_ActiveProject;
		}
		return nullptr;
	}

	bool CCProject::SaveActive(const std::filesystem::path& path)
	{
		CCProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}
		return false;
	}
}
