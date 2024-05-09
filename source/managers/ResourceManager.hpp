#pragma once
#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/Singleton.hpp>
#include <unordered_map>

namespace brk
{
	template <class Res>
	class ResourceRef
	{
	public:
		using TMutableRes = std::remove_const_t<Res>;
		static_assert(std::is_base_of_v<Resource, TMutableRes>, "Invalid resource type");

		ResourceRef(TMutableRes& ptr);
		ResourceRef(const ResourceRef&);
		ResourceRef(ResourceRef&&) noexcept;
		~ResourceRef();

		[[nodiscard]] Res* operator->();
		[[nodiscard]] Res& operator*();

	private:
		TMutableRes* m_Ptr;
	};

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager() noexcept = default;

	private:
		friend class Singleton<ResourceManager>;
		template<class T>
		friend class ResourceRef;

		void Unload(Resource* res);

		ResourceManager() noexcept = default;

		TULIDMap<Resource*> m_Resources;
	};
} // namespace brk

#include "ResourceManager.inl"