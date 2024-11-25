#include <core/ULIDFormatter.hpp>

template <class R>
void brk::ResourceManager::RegisterResourceType()
{
	static_assert(
		std::is_base_of_v<Resource, R> && meta::HasName<R> &&
			std::is_constructible_v<R, const ULID&>,
		"Invalid resource type");

	constexpr uint32 h = Hash<StringView>{}(R::Name);
	m_TypeMap.emplace(
		h,
		[](const ULID id) -> Resource*
		{
			return static_cast<Resource*>(new R{ id });
		});
}

template <class R, class... Args>
R& brk::ResourceManager::AddResource(Args&&... args)
{
	static_assert(std::is_base_of_v<Resource, R>, "R must inherit from Resource");
	R* res = new R{ std::forward<Args>(args)... };
	Resource* temp = static_cast<Resource*>(res);
	BRK_ASSERT(temp->m_Id, "Newly created resource has invalid ULID");
	BRK_ASSERT(
		m_Resources.try_emplace(temp->m_Id, temp).second,
		"Couldn't add resource {} to the manager: ID already present",
		temp->m_Id);
	return *res;
}

template <class R>
brk::ResourceRef<R> brk::ResourceManager::GetRef(const ULID id)
{
	const auto it = m_Resources.find(id);
	DEBUG_CHECK(it != m_Resources.end())
	{
		return {};
	}
	using TRef = ResourceRef<R>;

	Resource* ptr = it->second;
	BRK_ASSERT(dynamic_cast<R*>(ptr), "Invalid cast for resource {}!", ptr->GetId());
	return TRef{ static_cast<R*>(ptr) };
}