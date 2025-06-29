#pragma once

#include "Cataclysm/Core/CCUUID.h"

namespace Cataclysm
{
	struct IDComponent
	{
		CCUUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		bool Enabled = true;
	};
}
