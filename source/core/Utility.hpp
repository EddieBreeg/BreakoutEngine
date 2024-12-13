#pragma once

#include <iterator>
#include <type_traits>

namespace brk {
	template <class T, class... U>
	constexpr auto Max(T&& left, U&&... right) noexcept
	{
		static_assert(
			(std::is_same_v<std::decay_t<T>, std::decay_t<U>> && ...),
			"All values must have the same type");
		T res = std::forward<T>(left);

		return ((res = res > right ? res : std::forward<U>(right)), ...);
	}

	template <class T, class... U>
	constexpr auto Min(T&& left, U&&... right) noexcept
	{
		static_assert(
			(std::is_same_v<std::decay_t<T>, std::decay_t<U>> && ...),
			"All values must have the same type");
		T res = std::forward<T>(left);

		return ((res = res < right ? res : std::forward<U>(right)), ...);
	}

	template <class T, size_t N>
	[[nodiscard]] constexpr size_t ArraySize(const T (&arr)[N]) noexcept
	{
		return N;
	}

	template <class Iter>
	struct Enumerator
	{
	public:
		using Traits = std::iterator_traits<Iter>;
		using Reference = typename Traits::reference;

		constexpr Enumerator(Iter start, Iter end)
			: m_Start{ start }
			, m_End{ end }
		{}

		constexpr Enumerator& operator++()
		{
			if (m_Start != m_End)
			{
				++m_Start;
				++m_Index;
			}
			return *this;
		}

		constexpr std::pair<Reference, uint32_t> operator*()
		{
			return { *m_Start, m_Index };
		}
		constexpr std::pair<const Reference, uint32_t> operator*() const
		{
			return { *m_Start, m_Index };
		}

		constexpr bool operator!=(const Enumerator& other) const
		{
			return m_Start != other.m_Start;
		}

		constexpr Enumerator& begin() noexcept { return *this; }
		constexpr Enumerator end() { return { m_End, m_End }; }

	private:
		Iter m_Start, m_End;
		uint32_t m_Index = 0;
	};

	template <class T>
	auto Enumerate(T&& container)
	{
		return Enumerator{ std::begin(container), std::end(container) };
	}
} // namespace brk