namespace brk::rdr {
	template <class P>
	inline void MaterialInstance::SetParameters(P&& params)
	{
		if (m_ParamBuffer)
		{
			m_ParamBuffer.SetData(params);
			return;
		}
		m_ParamBuffer = Buffer{
			Buffer::ParamBuffer,
			std::forward<P>(params),
			GetBufferOptions(m_BaseMat->m_Options.Get()),
		};
	}

	template <class P>
	inline MaterialInstance::MaterialInstance(
		ResourceRef<Material> baseMat,
		P&& params,
		const ULID& id,
		std::string name)
		: Resource(id, std::move(name), {})
		, m_BaseMat{ std::move(baseMat) }
		, m_ParamBuffer{
			Buffer::ParamBuffer,
			std::forward<P>(params),
			GetBufferOptions(m_BaseMat->m_Options.Get()),
		}
	{}

	inline EBufferOptions MaterialInstance::GetBufferOptions(
		MaterialSettings::EOptions matOptions)
	{
		return (matOptions & MaterialSettings::DynamicBufferParam)
				   ? EBufferOptions::Dynamic
				   : EBufferOptions::None;
	}
} // namespace brk::rdr
