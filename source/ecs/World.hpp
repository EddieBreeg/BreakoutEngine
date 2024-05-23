#pragma once

#include <core/MetaLists.hpp>
#include "Query.hpp"

namespace brk::ecs {
	/**
	 * Represents a view on the entity world. Every ECS system must declare the components
	 * it accesses using a specialization of this class.
	 * \tparam Components The list of components which can be accessed through this view.
	 * Mark them as const for read-only access.
	 */
	template <class... Components>
	class WorldView
	{
	public:
		using Access = meta::TypeList<Components...>;

		WorldView(entt::registry& world);
		WorldView(const WorldView&) = delete;
		WorldView(WorldView&&) = default;

		template <class C, class... Args>
		decltype(auto) AddComponent(const entt::entity e, Args&&... args);

		[[nodiscard]] entt::entity CreateEntity() { return m_EntityWorld.create(); }
		void DestroyEntity(const entt::entity e);

		template <class TQuery>
		QueryWorld<TQuery> Query();

	private:
		template <class C>
		static constexpr bool HasAccess =
			Access::Contains<C> || Access::Contains<std::remove_const_t<C>>;

		template <class Query, class = void>
		struct ValidateQuery : std::false_type
		{};

		template <class... I, class... E>
		struct ValidateQuery<
			QuerySpec<meta::TypeList<I...>, meta::TypeList<E...>>,
			std::enable_if_t<(HasAccess<I> && ...)>> : std::true_type
		{};

		entt::registry& m_EntityWorld;
	};

	namespace _internal {
		template <class T>
		struct IsWorldView : std::false_type
		{};
		template <class... C>
		struct IsWorldView<WorldView<C...>> : std::true_type
		{};
	} // namespace _internal

	template <class T>
	static constexpr bool IsWorldView = _internal::IsWorldView<T>::value;
} // namespace brk::ecs

#include "World.inl"