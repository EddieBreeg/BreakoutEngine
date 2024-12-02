#include "Material.hpp"
#include <core/DebugBreak.hpp>
#include <core/InputFile.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <managers/ResourceManager.hpp>
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

	Material::Material(
		const MaterialSettings& settings,
		const ULID& id,
		std::string name,
		std::string path)
		: Resource(id, std::move(name), std::move(path))
		, m_VertexShader{ InitShader<VertexShader>(settings.m_VertexSourceCode) }
		, m_FragmentShader{ InitShader<FragmentShader>(settings.m_FragmentSourceCode) }
		, m_Options{ settings.m_Options }
		, m_UseDefaultVertexShader{ false }
		, m_UseDefaultFragmentShader{ false }
	{
		// if all shaders were created successfully, no further loading is required
		if (m_VertexShader && m_FragmentShader)
			m_LoadingState = ELoadingState::Loaded;
	}

	bool Material::DoLoad() noexcept
	{
		if (m_UseDefaultVertexShader)
			m_VertexShader = VertexShader{};

		if (m_UseDefaultFragmentShader)
			m_FragmentShader = FragmentShader{};

		if ((m_VertexShader && m_FragmentShader))
			return true;

		/* If we don't have a file path, something went wrong, most likely a shader failed
		 * to compile*/
		if (m_FilePath.empty())
			return false;

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

	void Material::DoUnload() noexcept
	{
		/** If the material wasn't loaded from a file, do not actually unload anything,
		 * as we won't be able to load it later if we need to
		 */
		if (m_FilePath.empty())
			return;

		m_VertexShader.Reset();
		m_FragmentShader.Reset();
	}

	MaterialInstance::MaterialInstance(
		ResourceRef<Material> baseMat,
		const ULID& id,
		std::string name)
		: Resource(id, std::move(name), {})
		, m_BaseMat{ std::move(baseMat) }
	{
		m_ResourceIds[0] = m_BaseMat->GetId();
	}

	MaterialInstance::~MaterialInstance() {}

	void MaterialInstance::SetTexture(uint32 slot, ResourceRef<Texture2d> texture)
	{
		BRK_ASSERT(
			slot < s_MaxTextureCount,
			"Tried to bind texture to slot {}, valid indices range from 0 to {}",
			slot,
			s_MaxTextureCount - 1);
		m_ResourceIds[slot + 1] = texture->GetId();
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
			m_ResourceIds[1 + startSlot + i] = textures[i]->GetId();
			m_Textures[startSlot + i] = textures[i];
		}
	}

	bool MaterialInstance::DoLoad()
	{
		if (m_BaseMat)
			return true;

		ResourceManager& resourceManager = ResourceManager::GetInstance();
		m_BaseMat = resourceManager.GetRef<Material>(m_ResourceIds[0]);
		for (uint32 i = 0; i < s_MaxTextureCount; i++)
		{
			if (!m_ResourceIds[1 + i])
				continue;
			m_Textures[i] = resourceManager.GetRef<Texture2d>(m_ResourceIds[1 + i]);
		}

		return m_BaseMat;
	}

	void MaterialInstance::DoUnload()
	{
		if (!m_BaseMat)
			return;
		m_BaseMat.Reset();
		for (auto& texture : m_Textures)
			texture.Reset();
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
