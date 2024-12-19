namespace brk::ecs {
	template <class... Include, class... Exclude>
	inline QueryWorld<QuerySpec<meta::TypeList<Include...>, meta::TypeList<Exclude...>>>::
		QueryWorld(EntityWorld& world)
		: m_View{ world.view<Include...>(entt::exclude_t<Exclude...>{}) }
	{}
} // namespace brk::ecs
