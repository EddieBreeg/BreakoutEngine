#define BRK_DEV (BRK_DEBUG || BRK_EDITOR)

#include "Entry.hpp"

#include <ecs/ComponentRegistry.hpp>

#include <core/Loaders.hpp>
#include <core/Resource.hpp>

#include <managers/ResourceManager.hpp>

#include <entt/entity/registry.hpp>

#if BRK_DEV
#include <imgui/ComponentWidgets.hpp>
#include <imgui/MaterialWidgets.hpp>
#include <imgui/MeshWidget.hpp>
#include <imgui/TextureWidget.hpp>
#endif

#include <rendering/Material.hpp>
#include <rendering/Mesh.hpp>
#include <rendering/Texture.hpp>

#include <systems/TransformComponent.hpp>
#include <systems/VisualComponents.hpp>
#include "App.hpp"

void brk::App::RegisterComponents(const EntryPoint& entryPoint)
{
	auto& registry = ecs::ComponentRegistry::GetInstance();

	registry.Register<TransformComponent>();
	registry.Register<MeshComponent>();

	if (entryPoint.RegisterGameComponents)
		entryPoint.RegisterGameComponents(registry);
}

void brk::App::RegisterResources(const EntryPoint& entryPoint, ResourceManager& manager)
{
#if BRK_DEV
	manager.RegisterResourceType<rdr::Material, rdr::MaterialWidget>("material");
	manager.RegisterResourceType<rdr::MaterialInstance, rdr::MaterialInstanceWidget>(
		"materialInstance");
	manager.RegisterResourceType<rdr::Texture2d, rdr::Texture2dWidget>("texture2d");
	manager.RegisterResourceType<rdr::Mesh, rdr::MeshWidget>("mesh");
#else
	manager.RegisterResourceType<rdr::Material>("material");
	manager.RegisterResourceType<rdr::MaterialInstance>("materialInstance");
	manager.RegisterResourceType<rdr::Texture2d>("texture2d");
	manager.RegisterResourceType<rdr::Mesh>("mesh");
#endif
	if (entryPoint.RegisterGameResources)
		entryPoint.RegisterGameResources(manager);
}