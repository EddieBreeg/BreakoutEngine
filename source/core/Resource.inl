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

	template <class R, class Widget>
	inline ResourceTypeInfo ResourceTypeInfo::Create(StringView name)
	{
		ResourceTypeInfo info{
			name,
			[](const ULID id) -> Resource*
			{
				const ResourceAllocator<R> alloc;
				R* ptr = alloc.Allocate();
				return new (ptr) R{ id };
			},
			[](Resource* ptr)
			{
				ptr->~Resource();
				const ResourceAllocator<R> alloc;
				alloc.Deallocate(static_cast<R*>(ptr));
			},
		};
#ifdef BRK_EDITOR
		if constexpr (!std::is_void_v<Widget>)
		{
			info.m_Widget = new Widget{};
		}
#endif
		if constexpr (meta::IsComplete<JsonLoader<R>>)
		{
			info.m_Load = [](Resource& res, const nlohmann::json& json)
			{
				return JsonLoader<R>::Load(static_cast<R&>(res), json);
			};
			info.m_Save = [](const Resource& res, nlohmann::json& json)
			{
				JsonLoader<R>::Save(static_cast<const R&>(res), json);
			};
		}
		return info;
	}

	template <class R>
	inline R* ResourceAllocator<R>::Allocate() const
	{
		return static_cast<R*>(s_Provider.allocate(sizeof(R), alignof(R)));
	}
} // namespace brk
