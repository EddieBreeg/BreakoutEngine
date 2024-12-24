#include <core/StringView.hpp>

#ifdef BRK_EDITOR
#include <tools/DataEmbed.hpp>
#include <imgui.h>

#include <Montserrat.hpp>

namespace brk::editor {
	void ImportEditorFonts(ImGuiIO& io, float dpiScale)
	{
		std::vector data = tools::UncompressData(
			fonts::s_Montserrat.m_Data,
			uint32(sizeof(fonts::s_Montserrat.m_Data)),
			fonts::s_Montserrat.m_UncompressedSize);
		BRK_ASSERT(
			data.size() == fonts::s_Montserrat.m_UncompressedSize,
			"Failed to import Caviar Dream editor font");
		ImFontConfig config = {};
		config.FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF(
			data.data(),
			int32(data.size()),
			float(int32(16 * dpiScale)),
			&config);
	}
} // namespace brk::editor
#endif