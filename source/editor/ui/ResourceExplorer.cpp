#include "UiData.hpp"

#ifdef BRK_EDITOR

#include <core/Resource.hpp>
#include <managers/ResourceManager.hpp>
#include <imgui.h>

namespace {
	static constexpr const char* s_DefaultResName = "Unnamed";

	const brk::ResourceTypeInfo* ResourceTypeDropDown(
		const brk::ResourceTypeInfo* current,
		const brk::TResourceTypeMap& types)
	{
		const char* typeName = current ? current->m_TypeName.GetPtr() : "[none]";
		if (!ImGui::BeginCombo("Resource Type", typeName))
			return current;
		for (const auto& [hash, info] : types)
		{
			bool selected = &info == current;
			if (!ImGui::Selectable(info.m_TypeName.GetPtr(), &selected))
				continue;

			ImGui::EndCombo();
			return &info;
		}

		ImGui::EndCombo();
		return current;
	}
} // namespace

void brk::editor::ui::UiData::ResourceExplorer()
{
	ImGui::Begin("Resource Explorer");

	ImGui::BeginDisabled(m_ShowResourceCreationWindow);
	m_ShowResourceCreationWindow |= ImGui::Button("Create Resource");
	ImGui::EndDisabled();

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
		delete data.m_Resource;
		data.m_Resource = nullptr;
		data.m_Info = nullptr;
		goto RES_CREATION_END;
	}
	const auto* info = ResourceTypeDropDown(data.m_Info, resourceManager.GetTypeMap());
	if (info != data.m_Info)
	{
		delete data.m_Resource;
		data.m_Resource = nullptr;
	}
	data.m_Info = info;
	if (!data.m_Info)
		goto RES_CREATION_END;

	if (!data.m_Resource)
	{
		data.m_Resource = data.m_Info->m_Constructor(ULID::Generate());
	}

	const bool ready = data.m_Resource->CreationUiWidget();

	ImGui::BeginDisabled(!ready);
	m_AddResourceRequested = ImGui::Button("Create");
	m_ShowResourceCreationWindow = !m_AddResourceRequested;
	ImGui::EndDisabled();

RES_CREATION_END:
	ImGui::End();
	return nullptr;
}
#endif