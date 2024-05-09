#include "ResourceManager.hpp"
template <class Res>
brk::ResourceRef<Res>::ResourceRef(Res* ptr)
	: m_Ptr{ ptr }
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
		delete m_Ptr;
	}
}

template <class Res>
const Res* brk::ResourceRef<Res>::operator->()
{
	return m_Ptr;
}

template <class Res>
const Res& brk::ResourceRef<Res>::operator*()
{
	return *m_Ptr;
}
