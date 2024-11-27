#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include <ecs/System.hpp>
#include <vector>

namespace brk {
	class Resource;

	struct ResourceLoadingRequests
	{
		std::vector<Resource*> m_LoadRequests;
		std::vector<Resource*> m_UnloadRequests;

		static ResourceLoadingRequests s_Instance;

	private:
		ResourceLoadingRequests() = default;
		ResourceLoadingRequests(const ResourceLoadingRequests&) = delete;
	};

	/** WIP: This system takes care of all deferred resource loading/unloading requests.
	 * This will eventually be offloaded to a separate thread
	 */
	class ResourceLoadingSystem
	{
	public:
		using World = ecs::WorldView<>;

		ResourceLoadingSystem() = default;
		~ResourceLoadingSystem() = default;

		void Update(World& world, const TimeInfo&);

	private:
	};
} // namespace brk
