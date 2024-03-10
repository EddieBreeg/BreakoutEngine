#pragma once

#include <PCH.hpp>

namespace breakout::meta {

	template <auto... Vals>
	struct ValueList
	{
		template <size_t I>
		static constexpr auto Get()
		{
			using TSplit = Split<Vals...>;
			if constexpr (I >= sizeof...(Vals))
			{
				return;
			}
			else if constexpr (I == 0)
			{
				return TSplit::First;
			}
			else
			{
				return TSplit::Rest::template Get<I - 1>();
			}
		}

	private:
		template <auto...>
		struct Split;

		template <auto V0, auto... V>
		struct Split<V0, V...>
		{
			static constexpr auto First = V0;
			using Rest = ValueList<V...>;
		};

		template <>
		struct Split<>
		{};
	};

	template <class... Types>
	struct TypeList
	{
	private:
		template <class T0, class... T>
		struct Split
		{
			using First = T0;
			using Rest = TypeList<T...>;
		};
		template <size_t I>
		struct _Get
		{
			using Type = typename Split<Types...>::Rest::template _Get<I - 1>::Type;
		};
		template <>
		struct _Get<0>
		{
			using Type = typename Split<Types...>::First;
		};

	public:
		template <size_t I>
		using Get = typename _Get<I>::Type;
	};

	template <>
	struct TypeList<>
	{};

	template <class T, T... Values>
	static constexpr auto ConstMax()
	{
		T res = ValueList<Values...>::template Get<0>();
		return ((res = (res > Values ? res : Values)), ...);
	}
} // namespace breakout::meta
