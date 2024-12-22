#pragma once
#include <PCH.hpp>
#include <core/AllocTracker.hpp>
#include <core/Loaders.hpp>
#include <core/ResourceRef.hpp>
#include <core/Singleton.hpp>
#include <ecs/WorldFwd.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace brk {
	using TResourceTypeMap = std::unordered_map<uint32, ResourceTypeInfo*, Hash<uint32>>;

	class BRK_MANAGERS_API ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();

		/**
		 * Registers a new resource type. This is required in order to instanciate this
		 * resource at runtime.
		 * \details R must meet the requirements of IsResourceType (see Resource.hpp)
		 */
		template <class R, class Widget = void>
		void RegisterResourceType(const StringView name);

#ifdef BRK_EDITOR
		/**
		 * \short Pre-allocates the resource objects and adds them to the internal map,
		 * without loading the actual data.
		 * \param list: The list of resource descriptions to load. If a resource is
		 * already present in the map, it is ignored.
		 * \attention All relevant resource types must be registered using
		 * RegisterResourceType before calling this function. Will assert if this
		 * condition isn't met.
		 */
		void CreateResources(const nlohmann::json& list);
#endif

		[[nodiscard]] ResourceTypeInfo& GetResourceTypeInfo(StringView typeName) const;
#ifdef BRK_EDITOR
		[[nodiscard]] const TResourceTypeMap& GetTypeMap() const noexcept
		{
			return m_TypeMap;
		}
		[[nodiscard]] const TULIDMap<Resource*>& GetResources() const noexcept
		{
			return m_Resources;
		}
#endif

#ifdef BRK_DEV
		[[nodiscard]] const AllocTracker& GetAllocTracker() const noexcept;
#endif

		/**
		 * \short Manually adds a new resource to the map. The object is heap allocated
		 * with new
		 * \tparam R: The type of resource to create
		 * \param args: The arguments to be fowarded to the constructor
		 * \return A reference to the newly created object. Said object is guaranteed to
		 * be in the same state as after the call to its constructor.
		 * \attention After constructing the resource object, this function will check the
		 * id is valid (and assert if it is not), and attempt to insert it into the map.
		 * It will assert if another object with the same ULID was already present
		 */
		template <class R, class... Args>
		R& AddResource(Args&&... args);
		/**
		 * Adds a resource to the map. After this call, the resource manager owns the
		 * pointer that was passed in; the object will be freed when the manager shuts
		 * down.
		 */
		void AddResource(Resource* res);
		/**
		 * Deletes a resource. The resource is marked for deletion, meaning it will be
		 * deleted when the last reference to it is destroyed, or immediately if no such
		 * reference exists. In either case, it is removed from the resource map
		 * immediately.
		 */
		void DeleteResource(Resource* res);

		/**
		 * \return A reference to the resource with the corresponding ID, or a null
		 * resource if such ID wasn't found.
		 */
		template <class R>
		ResourceRef<R> GetRef(const ULID id);

	private:
		static std::unique_ptr<ResourceManager> s_Instance;
		friend class Singleton<ResourceManager>;

		friend struct RetainTraits<Resource>;

		ResourceManager(ecs::EntityWorld& world) noexcept;

		TULIDMap<Resource*> m_Resources;

		TResourceTypeMap m_TypeMap;
#ifdef BRK_DEV
		AllocTracker m_AllocTracker;
#endif
		ecs::EntityWorld& m_World;
		std::shared_mutex m_Mutex;
	};

	template <class R>
	struct JsonLoader<ResourceRef<R>>
	{
		static bool Load(ResourceRef<R>& out_ref, const nlohmann::json& json);
		static void Save(const ResourceRef<R>& ref, nlohmann::json& out_json);
	};
} // namespace brk

#include "ResourceManager.inl"