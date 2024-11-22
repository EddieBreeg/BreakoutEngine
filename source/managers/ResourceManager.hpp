#pragma once
#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/RetainPtr.hpp>
#include <core/Singleton.hpp>
#include <entt/entity/fwd.hpp>
#include <unordered_map>

namespace brk {
	struct ResourceRetainTraits
	{
		static constexpr Retain_t DefaultAction = {};
		static void Increment(Resource* res);
		static void Decrement(Resource* res);
		static uint32 GetCount(const Resource* res);
	};

	template <class Res>
	using ResourceRef = RetainPtr<Res, ResourceRetainTraits>;

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();

		/**
		 * Registers a new resource type. This is required in order to instanciate this
		 * resource at runtime.
		 */
		template <class R>
		void RegisterResourceType();

#ifdef BRK_EDITOR
		/**
		 * \short Pre-allocates the resource objects and adds them to the internal map,
		 * without loading the actual data.
		 * \param list: The list of resource descriptions to load. If a resource is
		 * already present in the map, it is ignored.
		 * \attention All resource relevant resource types must be registered using
		 * RegisterResourceType before calling this function. Will assert if this
		 * condition isn't met.
		 */
		void CreateResources(const std::vector<nlohmann::json>& list);
#endif

		/**
		 * \return A reference to the resource with the corresponding ID, or a null
		 * resource if such ID wasn't found.
		 */
		template <class R>
		ResourceRef<R> GetRef(const ULID id);

	private:
		friend class Singleton<ResourceManager>;

		friend struct ResourceRetainTraits;

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