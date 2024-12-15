#pragma once

#include <PCH.hpp>
#include "Component.hpp"
#include <core/Assert.hpp>
#include <core/Hash.hpp>
#include <core/LoadersFwd.hpp>
#include <core/LogManager.hpp>
#include <core/Singleton.hpp>
#include <core/StringViewFormatter.hpp>

#include <unordered_map>

namespace brk::ecs {
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
		const ComponentInfo& Register();

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
		BRK_ECS_API const ComponentInfo& GetInfo(const StringView name) const;

	private:
		friend class Singleton<ComponentRegistry>;
		BRK_ECS_API static std::unique_ptr<ComponentRegistry> s_Instance;
		ComponentRegistry() = default;

		using TMap = std::unordered_map<uint32, const ComponentInfo*, Hash<uint32>>;
		TMap m_TypeMap;
	};
} // namespace brk::ecs

#include "ComponentRegistry.inl"