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

	/**
	 * Checks whether T matches a trait
	 * \tparam T: The type to be tested
	 * \tparam Trait: A template class which, for another type T, declares a public
	 * constexpr static boolean named 'value', equal to true if T matches some condition,
	 * false otherwise.
	 * \example HasTrait<int, std::is_integral> is true, whereas HasTrait<float,
	 * std::is_integral> is false
	 */
	template <class T, template <class> class Trait>
	static constexpr bool HasTrait = _internal::HasTrait<T, Trait>::value;

	/** A generic list of types */
	template <class... T>
	struct TypeList
	{
		/**
		 * Creates TypeList<T..., U...>
		 */
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
		/**
		 * If Other is itself list of types U..., creates TypeList<T..., U...>
		 */
		template <class Other>
		using Concat = typename ConcatImpl<Other>::Type;

		/**
		 * Returns the Ith element in the list, I being a 0-based index
		 */
		template <size_t I>
		using Get = typename GetImpl<I, T...>::Type;

		/**
		 * Transforms all types in the list
		 * \tparam Converter: A templated type such that for any type T, Converter<T> is a
		 * new type
		 */
		template <template <class> class Converter>
		using Transform = TypeList<Converter<T>...>;

		/**
		 * Creates a new list with only the types T such that HasTrait<T, Trait> is true
		 */
		template <template <class> class Trait>
		using Where = typename AppendIf<TypeList<>, Trait, T...>::Type;

		/**
		 * true if the list contains type U, false otherwise
		 */
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
		template <size_t I>
		static constexpr auto Get = GetImpl<I, V...>::Value;
	};
} // namespace brk::meta
