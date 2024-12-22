#include <core/ULIDFormatter.hpp>
#include "ResourceManager.hpp"

#ifdef BRK_DEV
inline const brk::AllocTracker& brk::ResourceManager::GetAllocTracker() const noexcept
{
	return m_AllocTracker;
}
#endif

template <class R, class W>
void brk::ResourceManager::RegisterResourceType(const StringView name)
{
	static_assert(meta::IsResourceType<R>, "Invalid resource type");

	ResourceTypeInfo& info = ResourceTypeInfo::InitFor<R, W>(name, &m_AllocTracker);
	const uint32 h = Hash<StringView>{}(name);

	m_TypeMap.emplace(h, &info);
}

template <class R, class... Args>
R& brk::ResourceManager::AddResource(Args&&... args)
{
	static_assert(std::is_base_of_v<Resource, R>, "R must inherit from Resource");
	ResourceTypeInfo& info = ResourceTypeInfo::GetFor<R>();
	R* res = static_cast<R*>(info.m_Pool.Allocate(1));
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

template <class R>
inline void brk::JsonLoader<brk::ResourceRef<R>>::Save(
	const ResourceRef<R>& ref,
	nlohmann::json& out_json)
{
	if (!ref)
	{
		out_json = nullptr;
		return;
	}

	out_json = ref->GetId();
}