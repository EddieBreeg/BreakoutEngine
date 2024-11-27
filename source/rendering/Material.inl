namespace brk::rdr {
	template <class P>
	inline Material::Material(P&& parameters, const MaterialSettings& settings)
		: Material(settings)
	{
		m_ParamBuffer = Buffer{
			Buffer::ParamBuffer,
			parameters,
			settings.m_Options.HasAny(MaterialSettings::DynamicBufferParam)
				? EBufferOptions::Dynamic
				: EBufferOptions::None,
		};
	}

	template <class P>
	inline void Material::SetParameters(P&& params)
	{
		if (m_ParamBuffer)
		{
			m_ParamBuffer.SetData(params);
		}
		else
		{
			m_ParamBuffer = Buffer{
				Buffer::ParamBuffer,
				params,
				EnumFlags<EBufferOptions>{
					m_Options.HasAny(MaterialSettings::DynamicBufferParam)
						? EBufferOptions::Dynamic
						: EBufferOptions::None,
				},
			};
		}
	}
} // namespace brk::rdr
