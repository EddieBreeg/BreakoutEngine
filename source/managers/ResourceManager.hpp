#pragma once
#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/Singleton.hpp>
#include <entt/entity/fwd.hpp>
#include <unordered_map>

namespace brk {
	template <class Res>
	class ResourceRef
	{
	public:
		using TMutableRes = std::remove_const_t<Res>;
		static_assert(std::is_base_of_v<Resource, TMutableRes>, "Invalid resource type");

		ResourceRef() noexcept = default;
		ResourceRef(TMutableRes& ref);
		ResourceRef(const ResourceRef&);
		ResourceRef(ResourceRef&&) noexcept;
		~ResourceRef();

		ResourceRef& operator=(const ResourceRef& other);
		ResourceRef& operator=(ResourceRef&& other) noexcept;

		[[nodiscard]] bool IsValid() const noexcept { return m_Ptr; }

		[[nodiscard]] Res* operator->();
		[[nodiscard]] Res& operator*();

	private:
		TMutableRes* m_Ptr = nullptr;
	};

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();

		template <class R>
		void RegisterResourceType();

#ifdef BRK_DEV
		/**
		 * \short Pre-allocates the resource objects and adds them to the internal map,
		 * without loading the actual data
		 * \param list: The list of resource descriptions to load. If a resource is
		 * already present in the map, it is ignored
		 */
		void CreateResources(const std::vector<nlohmann::json>& list);
#endif

		template <class R>
		ResourceRef<R> GetRef(const ULID id);

	private:
		friend class Singleton<ResourceManager>;
		template <class T>
		friend class ResourceRef;

		Resource* CreateResource(const StringView type, const ULID id) const;

		void LoadDeferred(Resource* res);
		void UnloadDeferred(Resource* res);

		ResourceManager(entt::registry& world) noexcept;

		TULIDMap<Resource*> m_Resources;

		using TTypeMap =
			std::unordered_map<uint32, Resource* (*)(const ULID), Hash<uint32>>;
		TTypeMap m_TypeMap;
		entt::registry& m_World;
	};
} // namespace brk

#include "ResourceManager.inl"