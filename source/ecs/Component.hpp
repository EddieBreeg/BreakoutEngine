#include "ComponentFwd.hpp"
#include <core/Loaders.hpp>
#include <entt/entity/registry.hpp>

namespace brk::ecs {
	namespace _internal {
		template <class T, class = void>
		struct HasComponentInfo : std::false_type
		{};
		template <class T>
		struct HasComponentInfo<
			T,
			std::enable_if_t<std::is_same_v<decltype(T::Info), const ComponentInfo>>>
			: std::true_type
		{};

		template <class T, class = void>
		struct HasComponentWidget : std::false_type
		{};
		template <class T>
		struct HasComponentWidget<
			T,
			std::void_t<
				decltype(ComponentUiWidget<T>{ std::declval<const T&>() }),
				std::enable_if_t<std::is_invocable_r_v<bool, ComponentUiWidget<T>, T&>>>>
			: std::true_type
		{};
	} // namespace _internal

	template <class C>
	inline ComponentInfo ComponentInfo::Create(const char* name)
	{
		ComponentInfo info{
			name,
			[](const nlohmann::json& json,
			   EntityWorld& world,
			   const entt::entity entity) -> void*
			{
				C comp{};
				if (!JsonLoader<C>::Load(comp, json))
					return nullptr;
				C& res = world.emplace<C>(entity, std::move(comp));
				return &res;
			},
			[](EntityWorld& world, const entt::entity entity) -> void*
			{
				C& res = world.emplace<C>(entity);
				return &res;
			},
		};
		if constexpr (_internal::HasComponentWidget<C>::value)
		{
			info.m_WidgetInfo = {
				[](const void* ptr) -> void*
				{
					const auto& comp = *static_cast<const C*>(ptr);
					return new ComponentUiWidget<C>{ comp };
				},
				[](void* ptr, EntityWorld& world, entt::entity entity)
				{
					auto& widget = *static_cast<ComponentUiWidget<C>*>(ptr);
					return widget(world.get<C>(entity));
				},
			};
		}
		return info;
	}
} // namespace brk::ecs
