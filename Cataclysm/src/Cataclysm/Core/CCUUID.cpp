#include "ccpch.h"
#include "Cataclysm/Core/CCUUID.h"

#include <random>

#include <unordered_map>

namespace Cataclysm
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	CCUUID::CCUUID() : m_UUID(s_UniformDistribution(s_Engine))
	{

	}

	CCUUID::CCUUID(uint64_t uuid) : m_UUID(uuid)
	{

	}
}
