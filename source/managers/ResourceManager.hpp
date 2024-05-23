#pragma once
#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/Singleton.hpp>
#include <entt/entity/fwd.hpp>
#include <unordered_map>

namespace brk {
	/**
	 * Wrapper around a resource object pointer. This allows to automatically load/unload
	 * the resource when necessary.
	 */
	template <class Res>
	class ResourceRef
	{
	public:
		using TMutableRes = std::remove_const_t<Res>;
		static_assert(std::is_base_of_v<Resource, TMutableRes>, "Invalid resource type");

		// Constructs a null resource
		ResourceRef() noexcept = default;
		/**
		 * Creates a new reference and increments the resource's internal ref count.
		 */
		ResourceRef(TMutableRes& ref);
		/**
		 * Copies a reference and increments the resource's internal ref count.
		 */
		ResourceRef(const ResourceRef&);
		/**
		 * Moves the pointer and sets the other reference to null.
		 */
		ResourceRef(ResourceRef&&) noexcept;
		/**
		 * If the resource pointer is not nullptr, decrements the resource's internal ref
		 * count.
		 */
		~ResourceRef();

		/**
		 * Copies the resource reference and increment the resource's internal ref count.
		 */
		ResourceRef& operator=(const ResourceRef& other);
		// Swaps *this with other.
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

		/**
		 * Registers a new resource type. This is required in order to instanciate this
		 * resource at runtime.
		 */
		template <class R>
		void RegisterResourceType();

#ifdef BRK_DEV
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