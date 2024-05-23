#include <PCH.hpp>
#include <core/MetaLists.hpp>
#include <entt/entity/registry.hpp>

namespace brk::ecs {
	template <class...>
	class WorldView;

	/**
	 * Queries components from a world view
	 * \tparam Includes A TypeList of components which must be present on the entities
	 * \tparam Excludes A TypeList of components which must not be present on the entities
	 */
	template <class Includes, class Excludes>
	struct QuerySpec;

	template <class... I, class... E>
	struct QuerySpec<meta::TypeList<I...>, meta::TypeList<E...>>
	{
		/**
		 * Adds components to the include list
		 */
		template <class... T>
		using Include = QuerySpec<meta::TypeList<I..., T...>, meta::TypeList<E...>>;

		/**
		 * Adds components to the exclude list
		 */
		template <class... T>
		using Exclude = QuerySpec<meta::TypeList<I...>, meta::TypeList<E..., T...>>;
	};

	namespace query {
		/**
		 * Creates a query spec which includes the provided components
		 */
		template <class... T>
		using Include = QuerySpec<meta::TypeList<T...>, meta::TypeList<>>;

		/**
		 * Creates a query spec which excludes the provided components
		 */
		template <class... T>
		using Exclude = QuerySpec<meta::TypeList<>, meta::TypeList<T...>>;
	} // namespace query

	template <class QueryType>
	class QueryWorld;

	/**
	 * Returned by calling Query on a WorldView
	 */
	template <class... Include, class... Exclude>
	class QueryWorld<QuerySpec<meta::TypeList<Include...>, meta::TypeList<Exclude...>>>
	{
		using TNativeView = decltype(std::declval<entt::registry>().view<Include...>(
			entt::exclude_t<Exclude...>{}));

		template <class...>
		friend class WorldView;

		QueryWorld(entt::registry& world);
		QueryWorld(const QueryWorld&) = delete;

	public:
		[[nodiscard]] auto begin() noexcept { return m_View.begin(); }
		[[nodiscard]] auto end() noexcept { return m_View.end(); }

		/**
		 * Gets a specific component from an entity.
		 * \attention Will crash if the entity doesn't have the component. ONLY use this
		 * on an entity from the QueryWorld itself.
		 */
		template <class Component>
		Component& Get(const entt::entity e)
		{
			static_assert(
				(std::is_same_v<Component, Include> || ...),
				"Component is not in the include list");
			return m_View.get<Component>(e);
		}

		/**
		 * Gets a specific component from an entity.
		 * \attention Will crash if the entity doesn't have the component. ONLY use this
		 * on an entity from the QueryWorld itself.
		 */
		template <class Component>
		const Component& Get(const entt::entity e) const
		{
			static_assert(
				(std::is_same_v<Component, Include> || ...),
				"Component is not in the include list");
			return m_View.get<Component>(e);
		}

	private:
		TNativeView m_View;
	};

} // namespace brk::ecs

#include "Query.inl"