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
		ResourceManager::GetInstance().Unload(m_Ptr);
	}
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
