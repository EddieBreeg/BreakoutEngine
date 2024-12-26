#include "MaterialWidgets.hpp"
#include "MiscWidgets.hpp"

#include <imgui.h>
#include <core/Resource.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Texture.hpp>
#include <SDL3/SDL_dialog.h>

namespace {
	constexpr SDL_DialogFileFilter s_HlslFilter = { "HLSL files", "hlsl" };
}

namespace brk::rdr {
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

		int32 stackId = 1;
		if (ImGui::CollapsingHeader("Base Material"))
		{
			ResourceSelector(
				"Materials",
				m_MatId,
				ResourceTypeInfo::GetFor<Material>(),
				false,
				stackId);
		}
		char headerLabel[] = "Texture 0";
		for (auto&& [id, index] : Enumerate(m_TexIds))
		{
			if (ImGui::CollapsingHeader(headerLabel))
			{
				ResourceSelector(
					"Textures",
					id,
					ResourceTypeInfo::GetFor<Texture2d>(),
					true,
					stackId);
			}
			headerLabel[8]++;
		}
		return (bool)m_MatId && m_Name.length();
	}

	bool MaterialInstanceWidget::EditionUi(const Resource& res, bool& out_shouldReload)
	{
		const auto& mat = static_cast<const MaterialInstance&>(res);
		if (!CreationUi())
			return false;

		if (mat.m_IsValid)
		{
			out_shouldReload = m_MatId != mat.m_BaseMat->GetId();
			for (auto&& [tex, index] : Enumerate(mat.m_Textures))
			{
				if (out_shouldReload)
					break;
				const ULID& id = m_TexIds[index];
				if (tex)
				{
					out_shouldReload |= id != tex->GetId();
					continue;
				}
				out_shouldReload |= id;
			}
		}
		else
		{
			out_shouldReload = m_MatId != mat.m_ResourceIds.m_MaterialId;
			for (auto&& [id, index] : Enumerate(mat.m_ResourceIds.m_TextureIds))
			{
				if (out_shouldReload)
					break;
				out_shouldReload |= m_TexIds[index] != id;
			}
		}
		m_ShouldReload = out_shouldReload;

		return out_shouldReload || m_Name != mat.m_Name;
	}

	void MaterialInstanceWidget::Commit(Resource& inout_res) const
	{
		auto& mat = static_cast<MaterialInstance&>(inout_res);
		mat.m_Name = m_Name;

		if (mat.m_IsValid && m_ShouldReload)
		{
			mat.m_BaseMat.Reset();
			for (auto& tex : mat.m_Textures)
				tex.Reset();
			mat.m_IsValid = false;
		}
		if (m_ShouldReload)
		{
			// then copy resource IDs
			mat.m_ResourceIds.m_MaterialId = m_MatId;
			for (auto&& [id, index] : Enumerate(m_TexIds))
			{
				mat.m_ResourceIds.m_TextureIds[index] = id;
			}
		}
	}

	bool MaterialInstanceWidget::ResourceSelector(
		const char* label,
		ULID& current,
		const ResourceTypeInfo& type,
		bool allowReset,
		int32& stackId)
	{
		if (current)
		{
			dev_ui::ULIDWidget("Selected", current, stackId++);
			if (!allowReset)
				goto SELECTOR_START;
			ImGui::SameLine();
			ImGui::PushID(stackId++);
			if (ImGui::Button("Reset Selection"))
				current = {};
			ImGui::PopID();
		}
	SELECTOR_START:
		ImGui::PushID(stackId++);
		const bool ret =
			dev_ui::ResourceFilterWidget(label, *m_Resources, current, nullptr, &type);
		ImGui::PopID();
		return ret;
	}
} // namespace brk::rdr
