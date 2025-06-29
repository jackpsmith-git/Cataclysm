#pragma once

namespace Cataclysm::Vesuvius
{
	class VSGraphicsContext
	{
	public:
		virtual ~VSGraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static CCScope<VSGraphicsContext> Create(void* window);
	};
}
