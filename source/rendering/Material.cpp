#include "Material.hpp"
#include <core/DebugBreak.hpp>
#include <core/InputFile.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <rendering/Texture.hpp>
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
	Material::Material(const ULID& id)
		: Resource(id)
		, m_VertexShader{ nullptr }
		, m_FragmentShader{ nullptr }
		, m_UseDefaultVertexShader{ false }
		, m_UseDefaultFragmentShader{ false }
	{}

	Material::Material(const MaterialSettings& settings, const ULID& id, std::string name)
		: Resource(id, std::move(name), {})
		, m_VertexShader{ InitShader<VertexShader>(settings.m_VertexSourceCode) }
		, m_FragmentShader{ InitShader<FragmentShader>(settings.m_FragmentSourceCode) }
		, m_Options{ settings.m_Options }
		, m_UseDefaultVertexShader{ false }
		, m_UseDefaultFragmentShader{ false }
	{}

	bool Material::DoLoad() noexcept
	{
		if (m_UseDefaultVertexShader)
			m_VertexShader = VertexShader{};

		if (m_UseDefaultFragmentShader)
			m_FragmentShader = FragmentShader{};

		if (m_VertexShader && m_FragmentShader)
			return true;

		InputByteStream stream = LoadFileContents();
		const std::vector<char>& contents =
			static_cast<InputByteBuf*>(stream.rdbuf())->GetContents();
		if (contents.empty())
			return false;

		const StringView source{ contents.data(), uint32(contents.size()) };

		m_VertexShader = VertexShader{ source };
		m_FragmentShader = FragmentShader{ source };
		return m_VertexShader && m_FragmentShader;
	}

	MaterialInstance::MaterialInstance(
		ResourceRef<Material> baseMat,
		const ULID& id,
		std::string name)
		: Resource(id, std::move(name), {})
		, m_BaseMat{ std::move(baseMat) }
	{}

	MaterialInstance::~MaterialInstance() {}

	void MaterialInstance::SetTexture(uint32 slot, ResourceRef<Texture2d> texture)
	{
		BRK_ASSERT(
			slot < s_MaxTextureCount,
			"Tried to bind texture to slot {}, valid indices range from 0 to {}",
			slot,
			s_MaxTextureCount - 1);
		m_Textures[slot] = std::move(texture);
	}

	void MaterialInstance::SetTextures(
		const ResourceRef<Texture2d>* textures,
		uint32 numTextures,
		uint32 startSlot)
	{
		DEBUG_CHECK((numTextures + startSlot) <= s_MaxTextureCount)
		{
			numTextures = s_MaxTextureCount - startSlot;
			BRK_LOG_WARNING(
				"Called MaterialInstance::SetTextures with {} textures starting at slot "
				"{}, but max is {}",
				numTextures,
				startSlot,
				s_MaxTextureCount);
		}
		for (uint32 i = 0; i < numTextures; i++)
		{
			m_Textures[startSlot + i] = textures[i];
		}
	}

	bool MaterialInstance::DoLoad()
	{
		return m_BaseMat;
	}
} // namespace brk::rdr

namespace brk {
	bool JsonLoader<rdr::Material, void>::Load(
		rdr::Material& out_mat,
		const nlohmann::json& json)
	{
		bool useDefaultVShader = false;
		bool useDefaultFShader = false;
		Visit("useDefaultVertexShader", json, useDefaultVShader);
		Visit("useDefaultVertexShader", json, useDefaultFShader);
		out_mat.m_UseDefaultVertexShader = useDefaultVShader;
		out_mat.m_UseDefaultFragmentShader = useDefaultFShader;
		return true;
	}
} // namespace brk
