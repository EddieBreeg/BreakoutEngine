#pragma once

#include <PCH.hpp>
#include <core/StringView.hpp>
#include <core/Function.hpp>
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>

namespace brk::ecs {
	template <class Component>
	struct ComponentUiWidget;

	/**
	 * Runtime information about a component type. Used for loading/unloading, as well as
	 * ui widgets
	 */
	struct ComponentInfo
	{
		StringView m_Name;
		/**
		 * This is called when loading a component from a scene file in the editor
		 */
		void* (*m_LoadJson)(const nlohmann::json&, entt::registry&, entt::entity) =
			nullptr;

		struct WidgetInfo
		{
			void* (*m_Create)(const void*) = nullptr;
			bool (*m_Display)(void*, entt::registry&, entt::entity) = nullptr;
		} m_WidgetInfo;

		template <class C>
		static ComponentInfo Create(const char* name);
	};
} // namespace brk::ecs
