#include "Material.hpp"
#include <core/DebugBreak.hpp>
#include <core/LogManager.hpp>
#include <fstream>
#include <vector>

namespace {
	/**
	 * If source is not empty, returns a new shader compiled from source. Otherwise,
	 * returns default shader
	 */
	template <class S>
	S InitShader(brk::StringView source)
	{
		S result = source.GetLen() ? S{ source } : S{};
		return result;
	}
} // namespace

namespace brk::rdr {
	Material::Material(
		StringView vShaderCode,
		StringView fShaderCode,
		EMaterialOptions options)
		: Resource(ULID::Generate())
		, m_VertexShader{ InitShader<VertexShader>(vShaderCode) }
		, m_FragmentShader{ InitShader<FragmentShader>(fShaderCode) }
		, m_Options{ options }
	{}
} // namespace brk::rdr
