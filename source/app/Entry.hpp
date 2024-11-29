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
	 * This struct represents the public entry point API. It allows the user to implement
	 * various initialization functions
	 */
	struct EntryPoint
	{
		/**
		 * User defined function, which is in charge of registering all the ECS systems
		 * for the running game
		 */
		void (*RegisterGameSystems)(ecs::Manager& ecsManager) = nullptr;
		/**
		 * User defined function, which is in charge of registering all ECS component
		 * types for the running game. This is required for things like UI widgets (editor
		 * only), as well as component loading from files.
		 */
		void (*RegisterGameComponents)(ecs::ComponentRegistry& registry) = nullptr;
		/**
		 * User defined function, in charge of registering all resource types used in the
		 * game. If you have static resources embedded in the source code, this is also
		 * the time to add them to the manager
		 */
		void (*RegisterGameResources)(ResourceManager& resManager) = nullptr;
	};

	/**
	 * This function MUST be defined somewhere. Its only job is to provide the engine with
	 * the game specific implementation of the entry point. Any and all function pointer
	 * in the returned object may be null
	 */
	extern EntryPoint CreateEntryPoint();
} // namespace brk
