#pragma once
#include <PCH.hpp>
#include <core/Loaders.hpp>
#include <core/ResourceRef.hpp>
#include <core/Singleton.hpp>
#include <entt/entity/fwd.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace brk {
	struct ResourceTypeInfo
	{
		StringView m_TypeName;
		Resource* (*m_Constructor)(const ULID) = nullptr;
		bool (*m_Load)(Resource&, const nlohmann::json&) = nullptr;
	};

	class BRK_MANAGERS_API ResourceManager : public Singleton<ResourceManager>
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
		 * \attention All relevant resource types must be registered using
		 * RegisterResourceType before calling this function. Will assert if this
		 * condition isn't met.
		 */
		void CreateResources(const nlohmann::json& list);
#endif

		const ResourceTypeInfo& GetResourceTypeInfo(StringView typeName);

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
		 * \return A reference to the resource with the corresponding ID, or a null
		 * resource if such ID wasn't found.
		 */
		template <class R>
		ResourceRef<R> GetRef(const ULID id);

	private:
		static std::unique_ptr<ResourceManager> s_Instance;
		friend class Singleton<ResourceManager>;

		friend struct RetainTraits<Resource>;

		ResourceManager(entt::registry& world) noexcept;

		TULIDMap<Resource*> m_Resources;

		using TTypeMap = std::unordered_map<uint32, ResourceTypeInfo, Hash<uint32>>;
		TTypeMap m_TypeMap;
		entt::registry& m_World;
		std::shared_mutex m_Mutex;
	};

	template <class R>
	struct JsonLoader<ResourceRef<R>>
	{
		static bool Load(ResourceRef<R>& out_ref, const nlohmann::json& json);
	};
} // namespace brk

#include "ResourceManager.inl"