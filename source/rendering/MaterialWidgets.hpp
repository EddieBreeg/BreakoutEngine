#include <core/EnumFlags.hpp>
#include <core/Resource.hpp>
#include <core/ULID.hpp>
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Texture.hpp>
#include <SDL3/SDL_dialog.h>

namespace {
	constexpr SDL_DialogFileFilter s_HlslFilter = { "HLSL files", "hlsl" };
}

namespace brk::rdr {
	class MaterialWidget : public ResourceUiWidget
	{
	public:
		MaterialWidget() = default;
		void Init(const Resource&) override;
		bool CreationUi() override;
		bool EditionUi(const Resource& resource, bool& out_shouldReload) override;

		void Commit(Resource& out_resource) const override;

	private:
		bool m_UseDefaultVertexShader = false;
		bool m_UseDefaultFragmentShader = false;
		EnumFlags<MaterialSettings::EOptions> m_Options;
	};

	class MaterialInstanceWidget : public ResourceUiWidget
	{
	public:
		MaterialInstanceWidget() = default;
		void Init(const Resource&) override;
		bool CreationUi() override;

		void Commit(Resource& out_res) const override;

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

	bool MaterialWidget::EditionUi(const Resource& resource, bool& out_shouldReload)
	{
		if (!CreationUi())
			return false;
		const auto& mat = static_cast<const Material&>(resource);
		out_shouldReload = m_Options != mat.m_Options ||
						   m_UseDefaultFragmentShader != mat.m_UseDefaultFragmentShader ||
						   m_UseDefaultVertexShader != mat.m_UseDefaultVertexShader ||
						   m_FilePath != mat.m_FilePath;
		return out_shouldReload || m_Name != mat.m_Name;
	}

	void MaterialWidget::Commit(Resource& out_resource) const
	{
		auto& mat = static_cast<Material&>(out_resource);
		mat.m_Name = m_Name;
		mat.m_FilePath = m_FilePath;
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

	void MaterialInstanceWidget::Commit(Resource& inout_res) const
	{
		auto& mat = static_cast<MaterialInstance&>(inout_res);
		mat.m_Name = m_Name;

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
} // namespace brk::rdr
