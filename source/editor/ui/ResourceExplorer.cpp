#include "UiData.hpp"

#ifdef BRK_EDITOR

#include <core/Resource.hpp>
#include <managers/ResourceManager.hpp>
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>

namespace {
	static constexpr const char* s_DefaultResName = "Unnamed";

	const brk::ResourceTypeInfo* ResourceTypeDropDown(
		const brk::ResourceTypeInfo& current,
		const brk::TResourceTypeMap& types)
	{
		const char* typeName =
			current.m_TypeName.GetLen() ? current.m_TypeName.GetPtr() : "[none]";
		if (!ImGui::BeginCombo("Resource Type", typeName))
			return nullptr;
		for (const auto& [hash, info] : types)
		{
			if (!info->m_Widget)
				continue;
			const bool isCurrent = info->m_Constructor == current.m_Constructor;
			bool selected = isCurrent;
			if (!ImGui::Selectable(info->m_TypeName.GetPtr(), &selected))
				continue;

			ImGui::EndCombo();
			return isCurrent ? nullptr : info;
		}

		ImGui::EndCombo();
		return nullptr;
	}
} // namespace

void brk::editor::ui::UiData::ResourceExplorer(const ResourceManager& resourceManager)
{
	ImGui::Begin("Resource Explorer");

	m_ShowResourceCreationWindow |= ImGui::Button("Create Resource");

	ImGui::BeginChild("Resources", { 0, 0 }, ImGuiChildFlags_Borders);

	char idStr[28];
	idStr[0] = '(';
	idStr[27] = ')';

	for (const auto& [id, res] : resourceManager.GetResources())
	{
		id.ToChars<sizeof(idStr), 1>(idStr);
		ImGui::SetNextItemAllowOverlap();
		const bool selected = ImGui::Selectable(
			res->GetName().c_str(),
			false,
			ImGuiSelectableFlags_AllowDoubleClick);
		if (selected && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			m_ResourceEditorData.m_Resource = res;
			m_ResourceEditorData.m_Info = res->GetTypeInfo();
			if (m_ResourceEditorData.m_Info.m_Widget)
				m_ResourceEditorData.m_Info.m_Widget->Init(*res);
		}
		ImGui::SameLine();
		ImGui::TextUnformatted(idStr, idStr + sizeof(idStr));
	}

	ImGui::EndChild();

	ImGui::End();
}

brk::Resource* brk::editor::ui::UiData::ResourceCreationWindow(
	ResourceManager& resourceManager)
{
	auto& data = m_ResourceCreationData;
	if (!ImGui::Begin("Create Resource", &m_ShowResourceCreationWindow))
	{
		goto RES_CREATION_END;
	}
	if (!m_ShowResourceCreationWindow)
	{
		// window was closed, cancel resource creation
		data.m_Resource.reset();
		data.m_Info = {};
		goto RES_CREATION_END;
	}

	const auto* info = ResourceTypeDropDown(data.m_Info, resourceManager.GetTypeMap());
	if (info)
	{
		data.m_Info = *info;
		data.m_Resource.reset();
	}

	if (!data.m_Info.m_Constructor)
		goto RES_CREATION_END;

	if (!data.m_Resource)
	{
		data.m_Resource.reset(data.m_Info.m_Constructor(ULID::Generate()));
		data.m_Info.m_Widget->Init(*data.m_Resource);
	}

	const bool ready = data.m_Info.m_Widget->CreationUi();

	ImGui::BeginDisabled(!ready);
	if (m_AddResourceRequested = ImGui::Button("Create"))
	{
		data.m_Info.m_Widget->Commit(*data.m_Resource);
	}
	ImGui::EndDisabled();

RES_CREATION_END:
	ImGui::End();
	return nullptr;
}

void brk::editor::ui::UiData::ResourceEditor()
{
	bool show = m_ResourceEditorData.m_Resource;
	if (!show)
		return;
	Resource* resource = m_ResourceEditorData.m_Resource;

	if (!ImGui::Begin("Resource Editor", &show))
	{
		goto RES_EDITOR_END;
	}

	if (!show)
	{
		m_ResourceEditorData.m_Resource = {};
		goto RES_EDITOR_END;
	}

	ImGui::Text(
		"Type: %.*s",
		m_ResourceEditorData.m_Info.m_TypeName.GetLen(),
		m_ResourceEditorData.m_Info.m_TypeName.GetPtr());

	if (!m_ResourceEditorData.m_Info.m_Widget)
		goto RES_EDITOR_END;

	const bool ready = m_ResourceEditorData.m_Info.m_Widget->EditionUi(*resource);

	ImGui::BeginDisabled(!ready);
	if (ImGui::Button("Apply"))
	{
		m_ResourceEditorData.m_Info.m_Widget->Commit(*resource);
	}
	ImGui::EndDisabled();

RES_EDITOR_END:
	ImGui::End();
}
#endif