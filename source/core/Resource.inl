namespace brk {
	template <class R, class Widget>
	inline ResourceTypeInfo ResourceTypeInfo::Create(StringView name)
	{
		ResourceTypeInfo info{
			name,
			[](const ULID id) -> Resource*
			{
				return new R{ id };
			},
		};
		if constexpr (!std::is_void_v<Widget>)
		{
			info.m_Widget = new Widget{};
		}
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
