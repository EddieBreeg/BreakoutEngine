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
		static_assert(std::is_base_of_v<Resource, Res>, "Invalid resource type");
		ResourceRef(Res* ptr);
		ResourceRef(const ResourceRef&);
		ResourceRef(ResourceRef&&) noexcept;
		~ResourceRef();

		[[nodiscard]] const Res* operator->();
		[[nodiscard]] const Res& operator*();

	private:
		Res* m_Ptr;
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