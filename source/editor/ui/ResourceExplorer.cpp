#include "UiData.hpp"

#ifdef BRK_EDITOR

#include <core/Resource.hpp>
#include <managers/ResourceManager.hpp>
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>

namespace {
	static constexpr const char* s_DefaultResName = "Unnamed";

	brk::ResourceTypeInfo* ResourceTypeDropDown(
		brk::ResourceTypeInfo* current,
		const brk::TResourceTypeMap& types)
	{
		const char* typeName = current ? current->m_TypeName.GetPtr() : "[none]";
		if (!ImGui::BeginCombo("Resource Type", typeName))
			return nullptr;

		for (const auto& [hash, info] : types)
		{
			if (!info->m_Widget)
				continue;

			const bool isCurrent = info == current;
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
	ImGui::Begin(s_StrResourceExplorer);

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
			m_ResourceEditorData.m_Info = &res->GetTypeInfo();
			if (m_ResourceEditorData.m_Info->m_Widget)
				m_ResourceEditorData.m_Info->m_Widget->Init(*res);
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

	auto* info = ResourceTypeDropDown(data.m_Info, resourceManager.GetTypeMap());
	if (info)
	{
		data.m_Info = info;
		data.m_Resource.reset();
	}

	if (!data.m_Info)
		goto RES_CREATION_END;

	if (!data.m_Resource)
	{
		data.m_Resource.reset(data.m_Info->NewResource(ULID::Generate()));
		data.m_Info->m_Widget->Init(*data.m_Resource);
	}

	const bool ready = data.m_Info->m_Widget->CreationUi();

	ImGui::BeginDisabled(!ready);
	if (m_AddResourceRequested = ImGui::Button("Create"))
	{
		data.m_Info->m_Widget->Commit(*data.m_Resource);
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
		m_ResourceEditorData.m_Resource = nullptr;
		goto RES_EDITOR_END;
	}

	ImGui::Text(
		"Type: %.*s",
		m_ResourceEditorData.m_Info->m_TypeName.GetLen(),
		m_ResourceEditorData.m_Info->m_TypeName.GetPtr());
	ImGui::SameLine();

	const bool disabled = resource->GetSavingDisabled();
	if (disabled)
	{
		ImGui::TextDisabled("(?)");
		ImGui::SetItemTooltip("Can't edit this resource: saving is disabled");
	}
	ImGui::BeginDisabled(disabled);
	m_ResourceEditorData.m_ReloadRequested = ImGui::Button("Reload");
	ImGui::SameLine();
	if (m_ResourceEditorData.m_ShowDeletionWarning |= ImGui::Button("Delete"))
	{
		ImGui::OpenPopup("Delete Resource?");
	}
	ImGui::EndDisabled();

	if (ImGui::BeginPopupModal(
			"Delete Resource?",
			&m_ResourceEditorData.m_ShowDeletionWarning))
	{
		const uint32 refCount = resource->GetRefCount();
		if (refCount)
		{
			ImGui::Text(
				"Are you sure you want to delete %s?\nIt is currently referenced"
				"%u time(s), all references will become invalid and will have to\n"
				"be fixed manually.",
				resource->GetName().c_str(),
				refCount);
		}
		else
		{
			ImGui::Text(
				"Are you sure you want to delete %s?",
				resource->GetName().c_str());
		}
		m_ResourceEditorData.m_DeletionRequested = ImGui::Button("Confirm");
		ImGui::SameLine();
		m_ResourceEditorData.m_ShowDeletionWarning =
			!ImGui::Button("Cancel") && !m_ResourceEditorData.m_DeletionRequested;
		ImGui::EndPopup();
	}

	if (!m_ResourceEditorData.m_Info->m_Widget)
		goto RES_EDITOR_END;

	bool shouldReload = false;
	ImGui::BeginDisabled(disabled);
	const bool ready =
		m_ResourceEditorData.m_Info->m_Widget->EditionUi(*resource, shouldReload);
	ImGui::EndDisabled();

	ImGui::BeginDisabled(!ready);
	if (ImGui::Button("Apply"))
	{
		m_ResourceEditorData.m_Info->m_Widget->Commit(*resource);
		m_ResourceEditorData.m_ReloadRequested = shouldReload;
		m_ResourceEditorData.m_SaveRequested = !resource->GetSavingDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		m_ResourceEditorData.m_Info->m_Widget->Init(*resource);
	}
	ImGui::EndDisabled();

RES_EDITOR_END:
	ImGui::End();
}
#endif