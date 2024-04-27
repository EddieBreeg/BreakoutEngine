#include "World.hpp"
namespace brk::ecs {
	template <class... Components>
	inline WorldView<Components...>::WorldView(entt::registry& world)
		: m_EntityWorld{ world }
	{}

	template <class... Components>
	inline void WorldView<Components...>::DestroyEntity(const entt::entity e)
	{
		m_EntityWorld.destroy(e);
	}

	template <class... Components>
	template <class C, class... Args>
	decltype(auto) WorldView<Components...>::AddComponent(const entt::entity e, Args&&... args)
	{
		return m_EntityWorld.emplace<C>(e, std::forward<Args>(args)...);
	}

	template <class... Components>
	template <class TQuery>
	QueryWorld<TQuery> WorldView<Components...>::Query()
	{
		return { m_EntityWorld };
	}
} // namespace brk::ecs
