#include "UiData.hpp"

#ifdef BRK_EDITOR

#include <core/Resource.hpp>
#include <managers/ResourceManager.hpp>
#include <imgui.h>

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
			bool selected = info->m_Constructor == current.m_Constructor;
			if (!ImGui::Selectable(info->m_TypeName.GetPtr(), &selected))
				continue;

			ImGui::EndCombo();
			return info;
		}

		ImGui::EndCombo();
		return nullptr;
	}
} // namespace

void brk::editor::ui::UiData::ResourceExplorer()
{
	ImGui::Begin("Resource Explorer");

	m_ShowResourceCreationWindow |= ImGui::Button("Create Resource");

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
		data.m_Info = *info;

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
#endif