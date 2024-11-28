#pragma once

namespace brk {
	class ResourceManager;
}

namespace brk::ecs {
	class ComponentRegistry;
	class Manager;
} // namespace brk::ecs

namespace brk {
	/**
	 * User defined function, which is in charge of registering all the ECS systems
	 * for the running game
	 */
	extern void RegisterGameSystems(ecs::Manager& ecsManager);
	/**
	 * User defined function, which is in charge of registering all ECS component types
	 * for the running game. This is required for things like UI widgets (editor only), as
	 * well as component loading from files.
	 */
	extern void RegisterGameComponents(ecs::ComponentRegistry& registry);
	/**
	 * User defined function, in charge of registering all resource types used in the
	 * game. If you have static resources embedded in the source code, this is also the
	 * time to add them to the manager
	 */
	extern void RegisterGameResources(ResourceManager& resManager);
} // namespace brk
