#pragma once

#include <PCH.hpp>

namespace brk::meta {
	namespace _internal {
		template <class T, template <class> class Trait, class = void>
		struct HasTrait : std::false_type
		{};

		template <class T, template <class> class Trait>
		struct HasTrait<T, Trait, std::enable_if_t<Trait<T>::value>> : std::true_type
		{};

		template <class T, template <class> class Trait>
		struct HasTrait<T, Trait, std::enable_if_t<Trait<T>::Value>> : std::true_type
		{};
	}; // namespace _internal

	template <class T, template <class> class Trait>
	static constexpr bool HasTrait = _internal::HasTrait<T, Trait>::value;

	template <class... T>
	struct TypeList
	{
		template <class... U>
		using Append = TypeList<T..., U...>;

	private:
		template <class>
		struct ConcatImpl;
		template <class... U>
		struct ConcatImpl<TypeList<U...>>
		{
			using Type = Append<U...>;
		};

		template <size_t I, class T0, class... R>
		struct GetImpl
		{
			using Type = typename GetImpl<I - 1, R...>::Type;
		};
		template <class T0, class... R>
		struct GetImpl<0, T0, R...>
		{
			using Type = T0;
		};

		template <class L, template <class> class Trait, class...>
		struct AppendIf;
		template <template <class> class Trait, class... L, class T0>
		struct AppendIf<TypeList<L...>, Trait, T0>
		{
			using Type = std::
				conditional_t<HasTrait<T0, Trait>, TypeList<L..., T0>, TypeList<L...>>;
		};
		template <template <class> class Trait, class T0, class T1, class... L, class... R>
		struct AppendIf<TypeList<L...>, Trait, T0, T1, R...>
		{
			using Type = std::conditional_t<
				HasTrait<T0, Trait>,
				typename AppendIf<TypeList<L..., T0>, Trait, T1, R...>::Type,
				typename AppendIf<TypeList<L...>, Trait, T1, R...>::Type>;
		};

	public:
		template <class Other>
		using Concat = typename ConcatImpl<Other>::Type;

		template <size_t I>
		using Get = typename GetImpl<I, T...>::Type;

		template <template <class> class Converter>
		using Transform = TypeList<Converter<T>...>;

		template <template <class> class Trait>
		using Where = typename AppendIf<TypeList<>, Trait, T...>::Type;

		template <class U>
		static constexpr bool Contains = std::disjunction_v<std::is_same<U, T>...>;
	};

	template <>
	struct TypeList<>
	{
		template <class... U>
		using Append = TypeList<U...>;

	private:
		template <class>
		struct ConcatImpl;
		template <class... U>
		struct ConcatImpl<TypeList<U...>>
		{
			using Type = Append<U...>;
		};

	public:
		template <class L>
		using Concat = typename ConcatImpl<L>::Type;

		template <template <class> class Converter>
		using Transform = TypeList<>;

		template <template <class> class Trait>
		using Where = TypeList<>;

		template <class U>
		static constexpr bool Contains = false;
	};

	template <auto... V>
	struct ValueList
	{
	private:
		template <size_t I, auto V0, auto... R>
		struct GetImpl
		{
			static constexpr auto Value = GetImpl<I - 1, R...>::Value;
		};
		template <auto V0, auto... R>
		struct GetImpl<0, V0, R...>
		{
			static constexpr auto Value = V0;
		};

	public:
		template<size_t I>
		static constexpr auto Get = GetImpl<I, V...>::Value;
	};
} // namespace meta
