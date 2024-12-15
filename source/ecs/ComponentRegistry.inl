template <class C>
const brk::ecs::ComponentInfo& brk::ecs::ComponentRegistry::Register()
{
	static_assert(
		_internal::HasComponentInfo<C>::value,
		"Component types must declare a static const ComponentInfo Info member");
	const ComponentInfo& info = C::Info;
	const uint32 h = Hash<StringView>{}(info.m_Name);
	DEBUG_CHECK(info.m_WidgetInfo.m_Create)
	{
		BRK_LOG_WARNING("Component {} doesn't have a UI widget", info.m_Name);
	}

	const std::pair res = m_TypeMap.emplace(h, &info);
	return *(res.first->second);
}

template <class C>
const brk::ecs::ComponentInfo& brk::ecs::ComponentRegistry::GetInfo() const
{
	static_assert(
		_internal::HasComponentInfo<C>::value,
		"Component types must declare a static const ComponentInfo Info member");
	static const uint32 h = Hash<StringView>{}(C::Info.m_Name);
	const auto it = m_TypeMap.find(h);
	BRK_ASSERT(
		it != m_TypeMap.end(),
		"Attempting to get ComponentInfo for component '{}', which wasn't registered",
		C::Name);
	return *it->second;
}