#include "Material.hpp"
#include <core/DebugBreak.hpp>
#include <core/IO.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ResourceFormatter.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Texture.hpp>
#include <fstream>
#include <vector>

#ifdef BRK_EDITOR
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>
#include <SDL3/SDL_dialog.h>
#endif

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
	const ResourceTypeInfo Material::Info =
		ResourceTypeInfo::Create<Material, MaterialWidget>("material");

	class MaterialInstanceWidget : public ResourceUiWidget
	{
	public:
		MaterialInstanceWidget() = default;
		void Init(const Resource&) override;
		bool CreationUi() override;

		void Commit(Resource& out_res) override;

	private:
		bool ResourceSelector(
			const char* label,
			ULID& current,
			const ResourceTypeInfo& type,
			int32 stackId);

		ULID m_MatId;
		ULID m_TexIds[8];
		const TULIDMap<Resource*>* m_Resources = nullptr;
	};

	const ResourceTypeInfo MaterialInstance::Info =
		ResourceTypeInfo::Create<MaterialInstance, MaterialInstanceWidget>(
			"materialInstance");

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
	{}

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

		DynamicArrayStream stream = LoadFileContents();
		const std::vector<char>& contents = stream.GetBuffer().m_Buf;
		if (contents.empty())
			return false;

		const StringView source{ contents.data(), uint32(contents.size()) };

		if (!m_VertexShader)
			m_VertexShader = VertexShader{ source };
		if (!m_FragmentShader)
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

	MaterialInstance::MaterialInstance(const ULID& id, std::string name)
		: Resource(id, std::move(name), {})
		, m_ResourceIds{}
	{}

	MaterialInstance::MaterialInstance(
		ResourceRef<Material> baseMat,
		const ULID& id,
		std::string name)
		: Resource(id, std::move(name), {})
		, m_BaseMat{ std::move(baseMat) }
		, m_Textures{}
		, m_IsValid{ (bool)m_BaseMat }
	{}

	MaterialInstance::~MaterialInstance()
	{
		if (!m_IsValid)
			return;
		m_BaseMat.Reset();
		for (auto& texture : m_Textures)
		{
			texture.Reset();
		}
	}

	void MaterialInstance::SetTexture(uint32 slot, ResourceRef<Texture2d> texture)
	{
		BRK_ASSERT(
			slot < s_MaxTextureCount,
			"Tried to bind texture to slot {} on material instance {}, valid "
			"indices range from 0 to {}",
			slot,
			static_cast<const Resource&>(*this),
			s_MaxTextureCount - 1);
		BRK_ASSERT(
			m_IsValid,
			"Called SetTexture on invalid material instance {}",
			static_cast<const Resource&>(*this));
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
			return;
		}
		BRK_ASSERT(
			m_IsValid,
			"Called SetTexture on invalid material instance {}",
			static_cast<const Resource&>(*this));
		for (uint32 i = 0; i < numTextures; i++)
			m_Textures[startSlot + i] = textures[i];
	}

	void MaterialInstance::SetTextureId(uint32 slot, const ULID& id)
	{
		BRK_ASSERT(
			slot < s_MaxTextureCount,
			"Tried to bind texture to slot {} on material instance {}, valid "
			"indices range from 0 to {}",
			slot,
			static_cast<const Resource&>(*this),
			s_MaxTextureCount - 1);
		if (!m_IsValid)
		{
			m_ResourceIds.m_TextureIds[slot] = id;
			return;
		}
		m_Textures[slot] = ResourceManager::GetInstance().GetRef<Texture2d>(id);
	}

	void MaterialInstance::SetTextureIds(const ULID* ids, uint32 numIds, uint32 startSlot)
	{
		DEBUG_CHECK((numIds + startSlot) <= s_MaxTextureCount)
		{
			numIds = s_MaxTextureCount - startSlot;
			BRK_LOG_WARNING(
				"Called MaterialInstance::SetTextures with {} textures starting at slot "
				"{}, but max is {}",
				numIds,
				startSlot,
				s_MaxTextureCount);
			return;
		}
		if (!m_IsValid)
		{
			for (uint32 i = 0; i < numIds; i++)
				m_ResourceIds.m_TextureIds[startSlot + i] = ids[i];
			return;
		}
		auto& resourceManager = ResourceManager::GetInstance();
		for (uint32 i = 0; i < numIds; i++)
			m_Textures[startSlot + i] = resourceManager.GetRef<Texture2d>(ids[i]);
	}

	bool MaterialInstance::DoLoad()
	{
		if (m_IsValid)
			return true;

		const auto temp = m_ResourceIds;
		m_ResourceIds = {};

		ResourceManager& resourceManager = ResourceManager::GetInstance();
		new (&m_BaseMat) ResourceRef<Material>{
			resourceManager.GetRef<Material>(temp.m_MaterialId),
		};
		m_IsValid = (bool)m_BaseMat;
		if (!m_IsValid)
			return false;

		for (uint32 i = 0; i < s_MaxTextureCount; i++)
		{
			const ULID texId = temp.m_TextureIds[i];
			if (!texId)
			{
				new (m_Textures + i) ResourceRef<Texture2d>{};
				continue;
			}
			new (m_Textures + i) ResourceRef<Texture2d>{
				resourceManager.GetRef<Texture2d>(texId),
			};
		}
		return true;
	}

	void MaterialInstance::DoUnload()
	{
		if (!m_IsValid)
			return;
		decltype(m_ResourceIds) temp = {};
		temp.m_MaterialId = m_BaseMat->GetId();
		m_BaseMat.Reset();

		for (uint32 i = 0; i < s_MaxTextureCount; ++i)
		{
			if (m_Textures[i])
			{
				temp.m_TextureIds[i] = m_Textures[i]->GetId();
				m_Textures[i].Reset();
			}
		}
		m_ResourceIds = temp;
		m_IsValid = false;
	}

	bool MaterialInstance::IsLoaded() const noexcept
	{
		if (!m_IsValid)
			return false;
		if (!m_BaseMat->IsLoaded())
			return false;
		for (auto& texture : m_Textures)
		{
			if (texture && !texture->IsLoaded())
				return false;
		}
		return true;
	}

#ifdef BRK_EDITOR
	constexpr SDL_DialogFileFilter s_HlslFilter = { "HLSL files", "hlsl" };

	void MaterialWidget::Init(const Resource& res)
	{
		const auto& mat = static_cast<const Material&>(res);
		m_Id = mat.m_Id;
		m_Name = mat.m_Name;
		m_FilePath = mat.m_FilePath;
		m_Options = mat.m_Options;
		m_UseDefaultVertexShader = mat.m_UseDefaultVertexShader;
		m_UseDefaultFragmentShader = mat.m_UseDefaultFragmentShader;
	}

	bool MaterialWidget::CreationUi()
	{
		dev_ui::ULIDWidget("ULID", m_Id);
		dev_ui::StdStringInput("Material Name", m_Name);

		dev_ui::FilePathInput("HLSL File", m_FilePath, true, &s_HlslFilter, 1);

		ImGui::Checkbox("Use Default Vertex Shader", &m_UseDefaultVertexShader);
		ImGui::SameLine();
		ImGui::Checkbox("Use Default Fragment Shader", &m_UseDefaultFragmentShader);

		using TOpts = MaterialSettings::EOptions;
		dev_ui::FlagCheckbox(
			"Dynamic Parameter Buffer",
			m_Options,
			MaterialSettings::DynamicBufferParam);
		dev_ui::FlagCheckbox(
			"No Face Culling",
			m_Options,
			MaterialSettings::NoFaceCulling);

		return m_Name.size() && m_FilePath.length();
	}

	void MaterialWidget::Commit(Resource& out_resource)
	{
		auto& mat = static_cast<Material&>(out_resource);
		mat.m_Name = std::move(m_Name);
		mat.m_FilePath = std::move(m_FilePath);
		mat.m_Options = m_Options;
		mat.m_UseDefaultVertexShader = m_UseDefaultVertexShader;
		mat.m_UseDefaultFragmentShader = m_UseDefaultFragmentShader;
	}

	void MaterialInstanceWidget::Init(const Resource& res)
	{
		if (!m_Resources)
		{
			m_Resources = &ResourceManager::GetInstance().GetResources();
		}

		const auto& mat = static_cast<const MaterialInstance&>(res);
		m_Id = mat.m_Id;
		m_Name = mat.m_Name;
		if (!mat.m_IsValid)
		{
			m_MatId = mat.m_ResourceIds.m_MaterialId;
			for (auto&& [id, index] : Enumerate(mat.m_ResourceIds.m_TextureIds))
			{
				m_TexIds[index] = id;
			}
			return;
		}

		m_MatId = mat.m_BaseMat->GetId();
		for (auto&& [tex, index] : Enumerate(mat.m_Textures))
		{
			if (tex)
			{
				m_TexIds[index] = tex->GetId();
			}
		}
	}

	bool MaterialInstanceWidget::CreationUi()
	{
		dev_ui::ULIDWidget("ULID", m_Id);
		dev_ui::StdStringInput("Instance Name", m_Name);

		if (ImGui::CollapsingHeader("Base Material"))
		{
			ResourceSelector("Materials", m_MatId, Material::Info, 1);
		}
		char headerLabel[] = "Texture 0";
		for (auto&& [id, index] : Enumerate(m_TexIds))
		{
			if (ImGui::CollapsingHeader(headerLabel))
			{
				ResourceSelector("Textures", id, Texture2d::Info, index + 1);
			}
			headerLabel[8]++;
		}

		return (bool)m_MatId && m_Name.length();
	}

	void MaterialInstanceWidget::Commit(Resource& inout_res)
	{
		auto& mat = static_cast<MaterialInstance&>(inout_res);
		mat.m_Name = std::move(m_Name);

		if (mat.m_IsValid)
		{
			// first reset all resource references if need be

			mat.m_BaseMat.Reset();
			for (auto& tex : mat.m_Textures)
				tex.Reset();
			mat.m_IsValid = false;
		}
		// then copy resource IDs

		mat.m_ResourceIds.m_MaterialId = m_MatId;
		for (auto&& [id, index] : Enumerate(m_TexIds))
		{
			mat.m_ResourceIds.m_TextureIds[index] = id;
		}
	}

	bool MaterialInstanceWidget::ResourceSelector(
		const char* label,
		ULID& current,
		const ResourceTypeInfo& type,
		int32 stackId)
	{
		if (current)
			dev_ui::ULIDWidget("Selected", current, stackId++);
		ImGui::PushID(stackId);
		const bool ret =
			dev_ui::ResourceFilterWidget(label, *m_Resources, current, nullptr, &type);
		ImGui::PopID();
		return ret;
	}
#endif

} // namespace brk::rdr

namespace brk {
	bool JsonLoader<rdr::Material, void>::Load(
		rdr::Material& out_mat,
		const nlohmann::json& json)
	{
		bool useDefaultVShader = false;
		bool useDefaultFShader = false;
		bool dynamicParams = false;
		bool noFaceCulling = false;
		Visit("useDefaultVertexShader", json, useDefaultVShader);
		Visit("useDefaultFragmentShader", json, useDefaultFShader);
		Visit("dynamicParamBuffer", json, dynamicParams);
		Visit("noFaceCulling", json, noFaceCulling);
		out_mat.m_UseDefaultVertexShader = useDefaultVShader;
		out_mat.m_UseDefaultFragmentShader = useDefaultFShader;
		if (dynamicParams)
			out_mat.m_Options.Set(rdr::MaterialSettings::DynamicBufferParam);
		if (noFaceCulling)
			out_mat.m_Options.Set(rdr::MaterialSettings::NoFaceCulling);

		return true;
	}

	void JsonLoader<rdr::Material>::Save(
		const rdr::Material& mat,
		nlohmann::json& out_json)
	{
		using rdr::MaterialSettings;
		if (mat.m_UseDefaultVertexShader)
			out_json["useDefaultVertexShader"] = true;
		if (mat.m_UseDefaultFragmentShader)
			out_json["useDefaultFragmentShader"] = true;
		if (mat.m_Options.HasAny(MaterialSettings::DynamicBufferParam))
			out_json["dynamicParamBuffer"] = true;
		if (mat.m_Options.HasAny(MaterialSettings::NoFaceCulling))
			out_json["noFaceCulling"] = true;
	}

	inline bool JsonLoader<rdr::MaterialInstance>::Load(
		rdr::MaterialInstance& out_mat,
		const nlohmann::json& json)
	{
		if (!Visit("material", json, out_mat.m_ResourceIds.m_MaterialId))
			return false;
		Visit("textures", json, out_mat.m_ResourceIds.m_TextureIds);
		return true;
	}

	void JsonLoader<rdr::MaterialInstance>::Save(
		const rdr::MaterialInstance& mat,
		nlohmann::json& out_json)
	{
		nlohmann::json& texArray = out_json["textures"] = nlohmann::json::array();
		if (mat.IsValid())
		{
			out_json["material"] = mat.m_BaseMat->GetId();
			for (const auto& tex : mat.m_Textures)
			{
				if (tex)
				{
					texArray.emplace_back(tex->GetId());
				}
				else
				{
					texArray.emplace_back(nlohmann::json{});
				}
			}
			return;
		}
		out_json["material"] = mat.m_ResourceIds.m_MaterialId;
		for (const ULID& id : mat.m_ResourceIds.m_TextureIds)
		{
			if (id)
				texArray.emplace_back(id);
			else
				texArray.emplace_back(nlohmann::json{});
		}
	}

} // namespace brk
