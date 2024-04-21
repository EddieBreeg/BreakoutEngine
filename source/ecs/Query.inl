namespace bre::ecs {
	template <class... Include, class... Exclude>
	inline QueryWorld<QuerySpec<meta::TypeList<Include...>, meta::TypeList<Exclude...>>>::
		QueryWorld(entt::registry& world)
		: m_View{ world.view<Include...>(entt::exclude_t<Exclude...>{}) }
	{}
} // namespace bre::ecs
