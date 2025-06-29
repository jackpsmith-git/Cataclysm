#pragma once

namespace Cataclysm
{
	class CCUUID
	{
	public:
		CCUUID();
		CCUUID(uint64_t uuid);
		CCUUID(const CCUUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<Cataclysm::CCUUID>
	{
		std::size_t operator()(const Cataclysm::CCUUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}
