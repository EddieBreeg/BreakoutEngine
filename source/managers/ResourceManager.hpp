#pragma once
#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/Singleton.hpp>
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
		void PreloadResources(const nlohmann::json& list);
#endif

		template <class R>
		ResourceRef<R> GetRef(const ULID id) const;

	private:
		friend class Singleton<ResourceManager>;
		template <class T>
		friend class ResourceRef;

		Resource* CreateResource(const nlohmann::json& desc) const;
		void UnloadDeferred(Resource* res);

		ResourceManager() noexcept = default;

		TULIDMap<Resource*> m_Resources;

		using TTypeMap = std::unordered_map<uint32, Resource* (*)(), Hash<uint32>>;
		TTypeMap m_TypeMap;
	};
} // namespace brk

#include "ResourceManager.inl"