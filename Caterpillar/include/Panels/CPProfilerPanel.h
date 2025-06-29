#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	struct CPTraceEvent
	{
		std::string Name;
		uint64_t Timestamp;
		uint64_t Duration;
		uint32_t ThreadID;
	};

	struct CPEventWithLevel
	{
		CPTraceEvent Event;
		int NestingLevel;

		CPEventWithLevel(const CPTraceEvent& e, int level)
			: Event(e), NestingLevel(level) {}
	};

	struct CPProfileSession
	{
		CPProfileSession(const std::string& name, const std::string& filepath)
			: Name(name), FilePath(filepath) {}

		std::string Name;
		std::string FilePath;
		std::vector<CPTraceEvent> Events;
		std::vector<uint32_t> Threads;
		uint64_t MinTimestamp = 0;
		uint64_t MaxTimestamp = 0;
		float Zoom = 0.01f;
		float OffsetX = 0.0f;

		bool _IsDraggingScroll = false;
		float _ScrollDragOffsetX = 0.0f;

		std::unordered_map<uint32_t, std::vector<CPEventWithLevel>> EventsByThread;
		float MaxNestingLevel = 1.0f;

		void Load();
		void ParseFile();
		void ExtractThreads();
		void ComputeNestingLevels();
	};

	class CPProfilerPanel
	{
	public:
		CPProfilerPanel() = default;
		~CPProfilerPanel() = default;

		void Init();

		void OnImGuiRender();
	private:
		std::vector<CPProfileSession> m_Sessions;
	};
}
