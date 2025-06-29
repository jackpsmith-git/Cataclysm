#include "ccpch.h"
#include "Cataclysm/Core/CCApplication.h"

#include "Cataclysm/Input/CCInput.h"
#include "Cataclysm/Vesuvius/VSRenderer.h"
#include "Cataclysm/Utils/CCPlatformUtils.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"

#ifdef CC_PLATFORM_WINDOWS
#include <Windows.h>
#include <ShObjIdl.h>
#endif

#include <filesystem>

namespace Cataclysm
{
	CCApplication* CCApplication::s_Instance = nullptr;

	CCApplication::CCApplication(const CCApplicationSpecification& specification)
		: m_Specification(specification)
	{

		CC_CORE_ASSERT(!s_Instance, "[CCApplication::CCApplication] CCApplication already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);
		m_Window = CCWindow::Create(CCWindowProps(m_Specification.Name));
		m_Window->SetEventCallback(CC_BIND_EVENT_FN(CCApplication::OnEvent));

#ifdef CC_PLATFORM_WINDOWS
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
#endif
		
		Vesuvius::VSRenderer::Init();

		m_ImGuiLayer = new CCImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	CCApplication::~CCApplication()
	{
		CCScriptEngine::Shutdown();
		Vesuvius::VSRenderer::Shutdown();
	}

	void CCApplication::PushLayer(CCLayer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void CCApplication::PushOverlay(CCLayer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void CCApplication::SubmitToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);
		m_MainThreadQueue.emplace_back(function);
	}

	void CCApplication::OnEvent(CCEvent& e)
	{
		CCEventDispatcher dispatcher(e);
		dispatcher.Dispatch<CCWindowCloseEvent>(CC_BIND_EVENT_FN(CCApplication::OnWindowClose));
		dispatcher.Dispatch<CCWindowResizeEvent>(CC_BIND_EVENT_FN(CCApplication::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void CCApplication::Run()
	{
		while (m_Running)
		{
			// CC_PROFILE_SCOPE("Frame");

			float time = CCTime::GetTime();
			CCTimestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			ExecuteMainThreadQueue();

			if (!m_Minimized)
			{
				{
					for (CCLayer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					for (CCLayer* layer : m_LayerStack)
						layer->OnImGuiRender(timestep);
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void CCApplication::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
	}

	void CCApplication::Close()
	{
		m_Running = false;
	}

	bool CCApplication::OnWindowClose(CCWindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}	
	
	bool CCApplication::OnWindowResize(CCWindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Vesuvius::VSRenderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void CCApplication::SetWindowTitle(const char* title)
	{
		m_Window->SetWindowTitle(title);
	}
}
