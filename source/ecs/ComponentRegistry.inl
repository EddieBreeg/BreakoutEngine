namespace brk::_internal {
	template <class C, class = void>
	struct HasName : std::false_type
	{};
	template <class C>
	struct HasName<
		C,
		std::enable_if_t<std::is_same_v<const char* const, decltype(C::Name)>>>
		: std::true_type
	{};
} // namespace brk::_internal

template <class C>
const brk::ecs::ComponentInfo& brk::ecs::ComponentRegistry::Register()
{
	static_assert(_internal::HasName<C>::value, "Component doesn't have a name");
	static constexpr uint32 h = ComputeHash<C>();
	DEBUG_CHECK(m_TypeMap.find(h) == m_TypeMap.end())
	{
		LogManager::GetInstance().Log(
			LogManager::Warning,
			"Trying to register component '{}' mutliple times!",
			C::Name);
		return;
	}
	return m_TypeMap.emplace(h, CreateInfo<C>()).first->second;
}

template <class C>
const brk::ecs::ComponentInfo& brk::ecs::ComponentRegistry::GetInfo() const
{
	static_assert(_internal::HasName<C>::value, "Component doesn't have a name");
	static constexpr uint32 h = ComputeHash<C>();
	const auto it = m_TypeMap.find(h);
	BRK_ASSERT(
		it != m_TypeMap.end(),
		"Attempting to get ComponentInfo for component '{}', which wasn't registered",
		C::Name);
	return it->second;
}

template <class C>
brk::ecs::ComponentInfo brk::ecs::ComponentRegistry::CreateInfo()
{
	return {
		[](const nlohmann::json& json, entt::registry& world, const entt::entity entity)
		{
			C comp{};
			JsonLoader<C>::Load(comp, json);
			world.emplace<C>(entity, std::move(comp));
		},
	};
}

template <class C>
constexpr uint32 brk::ecs::ComponentRegistry::ComputeHash() noexcept
{
	uint32_t hash = 0;
	for (const char* str = C::Name; *str; ++str)
	{
		hash += *str++;
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash *= 0x68f96d6d;
	hash ^= hash >> 7;
	hash *= hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}
