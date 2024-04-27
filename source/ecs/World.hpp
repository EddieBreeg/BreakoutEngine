#pragma once

#include <core/MetaLists.hpp>
#include "Query.hpp"

namespace brk::ecs {
	template <class... Components>
	class WorldView
	{
	public:
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