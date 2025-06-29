#pragma once

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCWindow.h"
#include "Cataclysm/Core/CCLayerStack.h"

#include "Cataclysm/Events/CCEvent.h"
#include "Cataclysm/Events/CCApplicationEvent.h"

#include "Cataclysm/Core/CCTimestep.h"

#include "Cataclysm/ImGui/CCImGuiLayer.h"

int main(int argc, char** argv);

namespace Cataclysm
{
	struct CCApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			CC_CORE_ASSERT("Index out of range!", index < Count);
			return Args[index];
		}
	};

	struct CCApplicationSpecification
	{
		std::string Name = "Cataclysm";
		std::string WorkingDirectory;
		CCApplicationCommandLineArgs CommandLineArgs;
	};

	class CCApplication
	{
	public:
		CCApplication(const CCApplicationSpecification& specification);
		virtual ~CCApplication();

		void OnEvent(CCEvent& e);

		void PushLayer(CCLayer* layer);
		void PushOverlay(CCLayer* layer);

		inline CCWindow& GetWindow() { return *m_Window; }

		void Close();

		CCImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static CCApplication& Get() { return *s_Instance; }

		const CCApplicationSpecification& GetSpecification() const { return m_Specification; }

		void SubmitToMainThread(const std::function<void()>& function);

		void SetWindowTitle(const char* title);
	private:
		void Run();
		bool OnWindowClose(CCWindowCloseEvent& e);
		bool OnWindowResize(CCWindowResizeEvent& e);

		void ExecuteMainThreadQueue();
	private:
		CCApplicationSpecification m_Specification;
		std::unique_ptr<CCWindow> m_Window;
		CCImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		CCLayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;
	private:
		static CCApplication* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	CCApplication* CreateApplication(CCApplicationCommandLineArgs args);
}
