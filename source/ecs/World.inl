namespace brk::ecs {
	template <class... Components>
	inline WorldView<Components...>::WorldView(EntityWorld& world)
		: m_EntityWorld{ world }
	{}

	template <class... Components>
	inline void WorldView<Components...>::DestroyEntity(const entt::entity e)
	{
		m_EntityWorld.destroy(e);
	}

	template <class... Components>
	template <class C, class... Args>
	decltype(auto) WorldView<Components...>::AddComponent(
		const entt::entity e,
		Args&&... args)
	{
		static_assert(!std::is_const_v<C>, "Trying to add const component type");
		static_assert(
			(std::is_same_v<C, Components> || ...),
			"Component is not in the access list");
		return m_EntityWorld.emplace<C>(e, std::forward<Args>(args)...);
	}

	template <class... Components>
	template <class TQuery>
	QueryWorld<TQuery> WorldView<Components...>::Query()
	{
		static_assert(ValidateQuery<TQuery>::value);
		return { m_EntityWorld };
	}
} // namespace brk::ecs
