#include <core/IO.hpp>
#include <core/StringView.hpp>
#include <tools/DataEmbed.hpp>
#include <fstream>
#include <sstream>
#include <Lorem.hpp>
#include <BrkIcon.hpp>

namespace {
	constexpr brk::StringView s_Lorem =
		R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut odio sem, semper et ex eu, dapibus ultricies massa. Sed ac maximus turpis. Praesent tellus quam, congue eu velit eu, mattis bibendum nibh. Praesent nec felis ut eros ullamcorper tincidunt non vitae felis. Sed pharetra diam consequat, pretium metus vitae, gravida tellus. Aliquam bibendum tellus magna, vel tempus nisl ultricies at. Donec euismod eros rutrum leo fermentum semper. Morbi rhoncus libero erat, at lacinia nulla luctus id. Phasellus a odio vel elit egestas bibendum sed sit amet tortor. Pellentesque placerat euismod vehicula. Etiam quam nulla, fermentum at aliquet in, pellentesque pellentesque urna. In venenatis dui sed quam tincidunt porta. Vivamus non felis ut neque facilisis elementum id eget velit. Sed a mauris vestibulum mauris semper commodo. Quisque bibendum felis mauris, vel laoreet ex blandit lacinia. Nulla rhoncus est at bibendum cursus. Fusce sed efficitur nisl.)";
} // namespace

int main(int argc, char const* argv[])
{
	{
		std::vector<char> data = brk::tools::DecodeEmbeddedData(
			brk::asset_embed::ut::s_Lorem.m_Data,
			brk::asset_embed::ut::s_Lorem.m_UncompressedSize);
		assert(data.size() == brk::asset_embed::ut::s_Lorem.m_UncompressedSize);
		const brk::StringView view{ data.data(), uint32(data.size()) };
		assert(view == s_Lorem);
	}
	{
		std::vector<char> data = brk::tools::DecodeEmbeddedData(
			brk::asset_embed::ut::s_BrkIcon.m_Data,
			brk::asset_embed::ut::s_BrkIcon.m_UncompressedSize);
		assert(data.size() == brk::asset_embed::ut::s_BrkIcon.m_UncompressedSize);
	}
	return 0;
}
