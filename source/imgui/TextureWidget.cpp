#include "TextureWidget.hpp"
#include "MiscWidgets.hpp"

#include <imgui.h>
#include <rendering/Texture.hpp>
#include <SDL3/SDL_dialog.h>

namespace {
	constexpr SDL_DialogFileFilter s_ImageFileFilters[] = {
		{ "Image Files", { "png;jpg;jpeg;bmp;gif;hdr" } },
	};

	constexpr const char* s_PixelFormatNames[] = { "Grayscale", "RGBA 8" };

	constexpr const char* s_FilterModeNames[] = { "Nearest", "Linear" };

	constexpr const char* s_AddressModeNames[] = { "Clamp", "Wrap", "Mirror", "Border" };
} // namespace

namespace brk::rdr {
	void brk::rdr::Texture2dWidget::Init(const Resource& res)
	{
		const auto& tex = static_cast<const Texture2d&>(res);
		m_Id = tex.m_Id;
		m_Name = tex.m_Name;
		m_FilePath = tex.m_FilePath;
		m_Settings = tex.m_Settings;
	}

	bool brk::rdr::Texture2dWidget::CreationUi()
	{
		dev_ui::ULIDWidget("ULID", m_Id);
		dev_ui::StdStringInput("Texture Name", m_Name);
		dev_ui::FilePathInput("Image File", m_FilePath, false, s_ImageFileFilters, 1);

		dev_ui::EnumDropDown(
			"Pixel Format",
			s_PixelFormatNames,
			(uint32)ArraySize(s_PixelFormatNames),
			m_Settings.m_Format);
		dev_ui::EnumDropDown(
			"Filter Mode",
			s_FilterModeNames,
			(uint32)ArraySize(s_FilterModeNames),
			m_Settings.m_FilterMode);
		dev_ui::EnumDropDown(
			"UV Address Mode",
			s_AddressModeNames,
			(uint32)ArraySize(s_AddressModeNames),
			m_Settings.m_UvAddressMode);
		if (m_Settings.m_UvAddressMode == EAddressMode::Border)
		{
			ImGui::ColorEdit4("Border Color", &m_Settings.m_BorderColor.x);
		}

		dev_ui::FlagCheckbox(
			"Is Shader Resource",
			m_Settings.m_Options,
			ETextureOptions::ShaderResource);
		ImGui::SameLine();
		dev_ui::FlagCheckbox(
			"Is Render Target",
			m_Settings.m_Options,
			ETextureOptions::RenderTarget);

		dev_ui::FlagCheckbox(
			"Is Dynamic",
			m_Settings.m_Options,
			ETextureOptions::Dynamic);

		return m_Name.length() && m_FilePath.length();
	}

	bool brk::rdr::Texture2dWidget::EditionUi(const Resource& res, bool& out_shouldReload)
	{
		if (!CreationUi())
			return false;
		const Texture2d& tex = static_cast<const Texture2d&>(res);
		out_shouldReload = m_Settings != tex.m_Settings || m_FilePath != tex.m_FilePath;
		return out_shouldReload || m_Name != tex.m_Name;
	}

	void brk::rdr::Texture2dWidget::Commit(Resource& out_res) const
	{
		auto& tex = static_cast<Texture2d&>(out_res);
		tex.m_Name = m_Name;
		tex.m_FilePath = m_FilePath;
		tex.m_Settings = m_Settings;
	}
} // namespace brk::rdr
