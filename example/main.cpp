#include <app/Entry.hpp>
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Material.hpp>
#include <rendering/Mesh.hpp>
#include <rendering/Texture.hpp>
#include "TestSystem.hpp"

namespace {
	static constexpr brk::rdr::Vertex3d s_PlaneVertices[] = {
		{ { -.5f, -.5f, 0 }, { 0, 0, 1 }, { 0, 0 } },
		{ { .5f, -.5f, 0 }, { 0, 0, 1 }, { 1, 0 } },
		{ { .5f, .5f, 0 }, { 0, 0, 1 }, { 1, 1 } },
		{ { -.5f, .5f, 0 }, { 0, 0, 1 }, { 0, 1 } },
	};
	static constexpr uint32 s_PlaneIndices[] = { 0, 1, 2, 2, 3, 0 };

	constexpr brk::ULID s_MeshId = brk::ULID::FromString("01JDYJJNF0R14DEZ74XR2YEK5G");

	void RegisterGameSystems(brk::ecs::Manager& ecsManager)
	{
		using namespace brk;

		ecsManager.AddSystem<sandbox::TestSystem>(
			ResourceManager::GetInstance(),
			ecsManager.GetWorld());
	}

	void RegisterGameResources(brk::ResourceManager& manager)
	{
		using namespace brk;

		manager
			.AddResource<rdr::Mesh>(s_PlaneVertices, s_PlaneIndices, s_MeshId, "plane");
	}
} // namespace

brk::EntryPoint brk::CreateEntryPoint()
{
	return { &RegisterGameSystems, nullptr, &RegisterGameResources };
}