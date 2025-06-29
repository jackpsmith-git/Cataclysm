#pragma once

namespace Cataclysm
{
	struct MonoScript
	{
		std::string ClassName;

		MonoScript() = default;
		MonoScript(const MonoScript&) = default;

		inline void Reset() {}
	};
}
