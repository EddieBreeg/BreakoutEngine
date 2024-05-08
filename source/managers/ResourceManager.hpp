#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/Singleton.hpp>

namespace brk
{
	template <class Res>
	class ResourceRef
	{
	public:
		static_assert(std::is_base_of_v<Resource, Res>, "Invalid resource type");
		ResourceRef(const ResourceRef&);
		ResourceRef(ResourceRef&&) noexcept;
		~ResourceRef();

		[[nodiscard]] const Res* operator->();
		[[nodiscard]] const Res& operator*();

	private:
		friend class ResourceManager;
		ResourceRef(Resource* ptr);

		Res* m_Ptr;
	};

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager() noexcept = default;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() noexcept = default;
	};
} // namespace brk

#include "ResourceManager.inl"