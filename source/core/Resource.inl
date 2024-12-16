namespace brk {
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
} // namespace brk
