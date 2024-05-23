#include <core/ULIDFormatter.hpp>

template <class Res>
brk::ResourceRef<Res>::ResourceRef(std::remove_const_t<Res>& ptr)
	: m_Ptr{ &ptr }
{
	++(m_Ptr->m_RefCount);
}

template <class Res>
brk::ResourceRef<Res>::ResourceRef(const ResourceRef& other)
	: m_Ptr{ other.m_Ptr }
{
	if (m_Ptr)
		++(m_Ptr->m_RefCount);
}

template <class Res>
brk::ResourceRef<Res>::ResourceRef(ResourceRef&& other) noexcept
	: m_Ptr{ other.m_Ptr }
{
	other.m_Ptr = nullptr;
}

template <class Res>
brk::ResourceRef<Res>::~ResourceRef()
{
	if (!m_Ptr)
		return;

	if (!--(m_Ptr->m_RefCount))
	{
		ResourceManager::GetInstance().UnloadDeferred(m_Ptr);
	}
}

template <class Res>
brk::ResourceRef<Res>& brk::ResourceRef<Res>::operator=(const ResourceRef& other)
{
	this->~ResourceRef();
	m_Ptr = other.m_Ptr;
	if (m_Ptr)
		++(m_Ptr->m_RefCount);
	return *this;
}

template <class Res>
brk::ResourceRef<Res>& brk::ResourceRef<Res>::operator=(ResourceRef&& other) noexcept
{
	std::swap(m_Ptr, other.m_Ptr);
	return *this;
}

template <class Res>
Res* brk::ResourceRef<Res>::operator->()
{
	return m_Ptr;
}

template <class Res>
Res& brk::ResourceRef<Res>::operator*()
{
	return *m_Ptr;
}

template <class R>
void brk::ResourceManager::RegisterResourceType()
{
	static_assert(
		std::is_base_of_v<Resource, R> && meta::HasName<R> &&
			std::is_default_constructible_v<R>,
		"Invalid resource type");

	constexpr uint32 h = Hash<StringView>{}(R::Name);
	m_TypeMap.emplace(
		h,
		[](const ULID id) -> Resource*
		{
			return new R{ id };
		});
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
	if (ptr->GetLoadingState() == Resource::Unloaded)
		LoadDeferred(ptr);

	return { static_cast<TRef::TMutableRes&>(*ptr) };
}