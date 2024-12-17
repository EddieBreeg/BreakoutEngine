#include <core/ULIDFormatter.hpp>

template <class R>
void brk::ResourceManager::RegisterResourceType()
{
	static_assert(meta::IsResourceType<R>, "Invalid resource type");

	const uint32 h = Hash<StringView>{}(R::Info.m_TypeName);

	m_TypeMap.emplace(h, &R::Info);
}

template <class R, class... Args>
R& brk::ResourceManager::AddResource(Args&&... args)
{
	static_assert(std::is_base_of_v<Resource, R>, "R must inherit from Resource");
	std::pmr::memory_resource* pool = R::Info.m_Pool;
	R* res = static_cast<R*>(pool->allocate(1));
	new (res) R{ std::forward<Args>(args)... };
	Resource* temp = static_cast<Resource*>(res);
	BRK_ASSERT(temp->GetId(), "Newly created resource has invalid ULID");
	temp->SetSavingDisabled();
	{
		std::unique_lock lock{ m_Mutex };
		BRK_ASSERT(
			m_Resources.try_emplace(temp->GetId(), temp).second,
			"Couldn't add resource {} to the manager: ID already present",
			temp->GetId());
	}
	return *res;
}

template <class R>
brk::ResourceRef<R> brk::ResourceManager::GetRef(const ULID id)
{
	std::shared_lock lock{ m_Mutex };
	const auto it = m_Resources.find(id);
	DEBUG_CHECK(it != m_Resources.end())
	{
		return {};
	}
	using TRef = ResourceRef<R>;

	Resource* ptr = it->second;
	lock.unlock();

	BRK_ASSERT(dynamic_cast<R*>(ptr), "Invalid cast for resource {}!", ptr->GetId());
	return TRef{ static_cast<R*>(ptr) };
}

template <class R>
inline bool brk::JsonLoader<brk::ResourceRef<R>>::Load(
	ResourceRef<R>& out_ref,
	const nlohmann::json& json)
{
	ULID id;
	json.get_to(id);
	DEBUG_CHECK(id)
	{
		BRK_LOG_ERROR(
			"Failed to load resource ref: invalid ULID ({})",
			nlohmann::to_string(json));
		return false;
	}
	out_ref = ResourceManager::GetInstance().GetRef<R>(id);
	return (bool)out_ref;
}