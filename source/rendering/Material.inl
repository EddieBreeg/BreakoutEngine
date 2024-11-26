namespace brk::rdr {
	template <class P>
	inline Material::Material(
		const P& parameters,
		StringView vShaderCode,
		StringView fShaderCode,
		EMaterialOptions options)
		: Material(vShaderCode, fShaderCode, options)
	{
		m_ParamBuffer = Buffer{
			Buffer::ParamBuffer,
			parameters,
			EnumFlags{ EBufferOptions::Dynamic },
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
				EnumFlags{ EBufferOptions::Dynamic },
			};
		}
	}
} // namespace brk::rdr
