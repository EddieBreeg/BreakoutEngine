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
	Material::Material(const MaterialSettings& settings, ULID id, std::string name)
		: Resource(id, std::move(name), {})
		, m_VertexShader{ InitShader<VertexShader>(settings.m_VertexSourceCode) }
		, m_FragmentShader{ InitShader<FragmentShader>(settings.m_FragmentSourceCode) }
		, m_Options{ settings.m_Options }
	{}

	MaterialInstance::MaterialInstance(
		ResourceRef<Material> baseMat,
		const ULID& id,
		std::string name)
		: Resource(id, std::move(name), {})
		, m_BaseMat{ baseMat }
	{}

} // namespace brk::rdr
