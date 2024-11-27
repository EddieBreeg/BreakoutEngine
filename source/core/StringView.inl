#include "StringView.hpp"
template <class Char>
constexpr brk::BasicStringView<Char>::BasicStringView(const Char* str, const uint32 len)
	: m_Ptr{ str }
	, m_Len{ len }
{}

template <class Char>
constexpr brk::BasicStringView<Char>::BasicStringView(const Char* str)
	: m_Ptr{ str }
{
	while (*str++)
		++m_Len;
}

template <class Char>
constexpr brk::BasicStringView<Char>::BasicStringView(const std::string& str)
	: m_Ptr{ str.c_str() }
	, m_Len{ (uint32)str.length() }
{}

template <class Char>
inline constexpr bool brk::BasicStringView<Char>::operator==(const BasicStringView& other)
{
	if (m_Len != other.m_Len)
		return false;
	for (uint32 i = 0; i < m_Len; i++)
	{
		if (m_Ptr[i] != other.m_Ptr[i])
			return false;
	}
	return true;
}

template <class Char>
inline constexpr bool brk::BasicStringView<Char>::operator!=(const BasicStringView& other)
{
	return !(*this == other);
}

template <class Char>
constexpr void brk::BasicStringView<Char>::TrimLeft(const uint32 n) noexcept
{
	if (n >= m_Len)
	{
		m_Ptr = nullptr;
		m_Len = 0;
	}
	else
	{
		m_Ptr += n;
		m_Len -= n;
	}
}

template <class Char>
inline constexpr uint32 brk::BasicStringView<Char>::Find(Char c) const noexcept
{
	for (uint32 i = 0; i < m_Len; i++)
	{
		if (m_Ptr[i] == c)
			return i;
	}
	return NPos;
}

template <class Char>
inline constexpr uint32 brk::BasicStringView<Char>::Find(
	BasicStringView sub) const noexcept
{
	if (sub.m_Len > m_Len)
		return NPos;
	const uint32 end = m_Len - sub.m_Len + 1;
	for (uint32 i = 0; i < end; i++)
	{
		bool found = true;
		for (uint32 j = 0; j < sub.m_Len; ++j)
		{
			if (m_Ptr[i + j] != sub.m_Ptr[j])
			{
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}
	return NPos;
}

template <class Char>
Char brk::BasicStringView<Char>::operator[](const uint32 index) const
{
	BRK_ASSERT(index < m_Len, "Index {} is out of range!", index);
	return m_Ptr[index];
}

constexpr uint32 brk::Hash<brk::StringView, void>::operator()(
	const StringView str) const noexcept
{
	uint32_t hash = 0;
	for (const char c : str)
	{
		hash += c;
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