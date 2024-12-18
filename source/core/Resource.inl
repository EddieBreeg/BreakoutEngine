#include "ResourceFwd.hpp"
namespace brk {
	inline bool Resource::DoLoad()
	{
		return false;
	}

	inline void Resource::DoUnload() {}

	inline bool Resource::IsLoaded() const noexcept
	{
		return GetLoadingState() == EStateFlags::Loaded;
	}

	inline void Resource::SetSavingDisabled(bool disabled) noexcept
	{
		const EStateFlags flagBit = EStateFlags(uint8(disabled) << 3);
		m_State = EStateFlags((m_State & ~SavingDisabled) | flagBit);
	}

	inline bool Resource::GetSavingDisabled() const noexcept
	{
		return (m_State & EStateFlags::SavingDisabled);
	}

	inline const ULID Resource::GetId() const noexcept
	{
		return m_Id;
	}

	inline const std::string& Resource::GetName() const noexcept
	{
		return m_Name;
	}

	inline const std::string& Resource::GetFile() const noexcept
	{
		return m_FilePath;
	}

	inline uint32 Resource::GetRefCount() const noexcept
	{
		return m_RefCount;
	}

	inline void Resource::SetLoadingState(EStateFlags state)
	{
		m_State = EStateFlags((m_State & ~LoadingStateMask) | state);
	}

	inline void ResourceUiWidget::Init(const Resource&) {}

	inline bool ResourceUiWidget::CreationUi()
	{
		return false;
	}

	inline bool ResourceUiWidget::EditionUi(const Resource& res, bool& out_shouldReload)
	{
		return false;
	}

	template <class R, class W>
	inline ResourceTypeInfo::ResourceTypeInfo(InPlaceType<R, W>, const StringView name)
		: m_TypeName{ name }
		, m_Pool{ InPlace<TypedMemoryPool<R>>, 100 }
	{
		m_Constructor = [](PolymorphicMemoryPool& pool, const ULID& id) -> Resource*
		{
			void* ptr = pool.Allocate(1);
			return new (ptr) R{ id };
		};
		if constexpr (!std::is_void_v<W>)
		{
			m_Widget = new W{};
		}
		if constexpr (meta::IsComplete<JsonLoader<R>>)
		{
			m_Load = [](Resource& res, const nlohmann::json& json)
			{
				return JsonLoader<R>::Load(static_cast<R&>(res), json);
			};
			m_Save = [](const Resource& res, nlohmann::json& json)
			{
				JsonLoader<R>::Save(static_cast<const R&>(res), json);
			};
		}
	}

	template <class Res, class Widget>
	inline ResourceTypeInfo& ResourceTypeInfo::InitFor(const StringView name)
	{
		if (Impl<Res>::s_Info)
			return *Impl<Res>::s_Info;

		Impl<Res>::s_Info.reset(new ResourceTypeInfo{ InPlace<Res, Widget>, name });
		return *Impl<Res>::s_Info;
	}

	template <class Res>
	inline void ResourceTypeInfo::ResetFor()
	{
		Impl<Res>::s_Info.reset();
	}

	template <class R>
	inline ResourceTypeInfo& ResourceTypeInfo::GetFor()
	{
		BRK_ASSERT(Impl<R>::s_Info, "Accessed unitialized resource type info");
		return *Impl<R>::s_Info;
	}
} // namespace brk
