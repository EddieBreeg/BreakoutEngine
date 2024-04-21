#include "World.hpp"

namespace bre::ecs {
	template <class... I, class... E>
	inline WorldView<meta::TypeList<I...>, meta::TypeList<E...>>::WorldView(
		entt::registry& world)
		: m_EntityWorld{ world }
		, m_NativeView{ world.view<I...>(entt::exclude_t<E...>{}) }
	{}

	template <class... I, class... E>
	template <class Component>
	inline Component& WorldView<meta::TypeList<I...>, meta::TypeList<E...>>::Get(
		entt::entity entity)
	{
		static_assert((std::is_same_v<Component, I> || ...),
					  "Component is not in the include list");
		return m_NativeView.get<Component>(entity);
	}

	template <class... I, class... E>
	template <class Component>
	inline Component& WorldView<meta::TypeList<I...>, meta::TypeList<E...>>::Get(
		entt::entity entity) const
	{
		static_assert((std::is_same_v<Component, I> || ...),
					  "Component is not in the include list");
		return m_NativeView.get<Component>(entity);
	}

} // namespace bre::ecs
