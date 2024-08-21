#pragma once

#include <PCH.hpp>
#include <core/Assert.hpp>
#include <core/Function.hpp>
#include <core/Hash.hpp>
#include <core/LoadersFwd.hpp>
#include <core/LogManager.hpp>
#include <core/Singleton.hpp>
#include <core/StringViewFormatter.hpp>
#include <entt/entity/fwd.hpp>

#include <unordered_map>

namespace brk::ecs {
	struct ComponentInfo
	{
		/**
		 * This is called when loading a component from a scene file in the editor
		 */
		void (*m_LoadJson)(const nlohmann::json&, entt::registry&, const entt::entity) =
			nullptr;
#ifdef BRK_DEV
		UniqueFunction<bool(entt::registry&, const entt::entity)> m_UiWidget;
#endif
		const char* m_Name;

	private:
		friend class ComponentRegistry;
		ComponentInfo() = default;
	};

	/**
	 * A global map containing type erased information about registered ECS components
	 */
	class ComponentRegistry : public Singleton<ComponentRegistry>
	{
	public:
		~ComponentRegistry() = default;

		/**
		 * Generates a ComponentInfo object for the provided type, and adds it to the map.
		 * \tparam C The component type. Must meet the meta::HasName trait.
		 * \param uiWidget: The function which displays the component
		 * \return A reference to the newly created info object, or the existing one if
		 * the component had already been registered.
		 */
		template <class C>
		const ComponentInfo& Register(bool (*uiWidget)(C&) = nullptr);

		/**
		 * Gets a component's info from its type
		 * \return A reference to the provided component's info object. Asserts if C had
		 * not been registered
		 */
		template <class C>
		const ComponentInfo& GetInfo() const;

		/**
		 * Gets a component's info from its name
		 * \return A reference to the component's info object. Asserts if the object
		 * wasn't found in the map
		 */
		const ComponentInfo& GetInfo(const StringView name) const;

	private:
		friend class Singleton<ComponentRegistry>;
		ComponentRegistry() = default;

		template <class C>
		static ComponentInfo CreateInfo(bool (*widget)(C&));

		using TMap = std::unordered_map<uint32, const ComponentInfo, Hash<uint32>>;
		TMap m_TypeMap;
	};
} // namespace brk::ecs

#include "ComponentRegistry.inl"