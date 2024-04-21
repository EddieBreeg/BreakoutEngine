#pragma once

#include <entt/entity/fwd.hpp>
#include <core/MetaLists.hpp>

namespace bre::ecs {
	template <class Includes, class Excludes>
	class WorldView;

	template <class... I, class... E>
	class WorldView<meta::TypeList<I...>, meta::TypeList<E...>>
	{
		using TNativeView = decltype(std::declval<entt::registry>().view<I...>(
			entt::exclude_t<E...>{}));

	public:
		WorldView(entt::registry& world);

		[[nodiscard]] entt::registry& GetEntityWorld() { return m_EntityWorld; }

		auto begin() { return m_NativeView.begin(); }
		auto end() { return m_NativeView.end(); }

		template <class Component>
		[[nodiscard]] Component& Get(entt::entity entity);

		template <class Component>
		[[nodiscard]] Component& Get(entt::entity entity) const;

	private:
		entt::registry& m_EntityWorld;
		TNativeView m_NativeView;
	};

	template <class... I>
	struct Include
	{
		using ViewType = WorldView<meta::TypeList<I...>, meta::TypeList<>>;

		template <class... E>
		struct Exclude
		{
			using ViewType = WorldView<meta::TypeList<I...>, meta::TypeList<E...>>;
		};
	};

	namespace _internal {
		template <class T>
		struct IsWorldView : std::false_type
		{};
		template <class... I, class... E>
		struct IsWorldView<WorldView<meta::TypeList<I...>, meta::TypeList<E...>>>
			: std::true_type
		{};
	} // namespace _internal

	template <class T>
	static constexpr bool IsWorldView = _internal::IsWorldView<T>::value;
} // namespace bre::ecs

#include "World.inl"