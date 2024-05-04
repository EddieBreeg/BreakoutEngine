#pragma once

#include <PCH.hpp>
#include <core/Assert.hpp>
#include <core/Hash.hpp>
#include <core/LoadersFwd.hpp>
#include <core/LogManager.hpp>
#include <core/Singleton.hpp>
#include <entt/entity/fwd.hpp>

#include <unordered_map>

namespace brk::ecs {
	struct ComponentInfo
	{
		void (*m_LoadJson)(const nlohmann::json&, entt::registry&, const entt::entity) =
			nullptr;
		const char* m_Name;

	private:
		friend class ComponentRegistry;
		ComponentInfo() = default;
	};	

	class ComponentRegistry : public Singleton<ComponentRegistry>
	{
	public:
		~ComponentRegistry() = default;

		template<class C>
		const ComponentInfo& Register();

		template <class C>
		const ComponentInfo& GetInfo() const;

	private:
		friend class Singleton<ComponentRegistry>;
		ComponentRegistry() = default;

		template <class C>
		static ComponentInfo CreateInfo();

		template<class C>
		static constexpr uint32 ComputeHash() noexcept;

		using TMap = std::unordered_map<uint32, const ComponentInfo, Hash<uint32>>;
		TMap m_TypeMap;
	};
} // namespace brk::ecs

#include "ComponentRegistry.inl"