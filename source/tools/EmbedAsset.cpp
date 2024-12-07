#include "DataEmbed.hpp"
#include <core/IO.hpp>
#include <fstream>

namespace {
	struct Settings
	{
		Settings(int argc, const char** argv)
			: m_AssetFilePath{ argv[1] }
			, m_SymbolName{ argv[2] }
		{
			if (argc > 3)
				m_NamespaceName = argv[3];
			if (argc > 4)
			{
				m_Base85 |= !std::strncmp("-b", argv[4], 2) ||
							!std::strncmp("--base85", argv[4], 8);
			}
		}

		const char* m_AssetFilePath;
		const char* m_SymbolName;
		const char* m_NamespaceName = "";
		bool m_Base85 = false;
	};

	int ExportBase85(const Settings& settings, std::vector<char>& data)
	{
		std::cout << "namespace " << settings.m_NamespaceName << R"({
	const struct { uint32 m_UncompressedSize; StringView m_Data; } s_)"
				  << settings.m_SymbolName << " = {" << data.size() << ", R\"~>(";

		if (!brk::tools::PackData(
				data.data(),
				uint32(data.size()),
				std::cout,
				brk::tools::EPackFormat::Base85))
			return 1;

		std::cout << ")~>\" };\n}";
		return 0;
	}

	int ExportArray(const Settings& settings, std::vector<char>& data)
	{
		brk::DynamicArrayStream out;
		std::vector<char>& compressed = out.GetBuffer().m_Buf;
		compressed.reserve(data.size());
		const uint32 cSize = brk::tools::PackData(data.data(), uint32(data.size()), out);
		if (!cSize)
			return 1;
		std::cout << "namespace " << settings.m_NamespaceName << R"({
	struct { uint32 m_UncompressedSize; char m_Data[)"
				  << cSize << "]; } s_" << settings.m_SymbolName << " = {" << data.size()
				  << ", {";
		for (char byte : compressed)
			std::cout << int32(byte) << ',';
		std::cout << "}};\n}";
		return 0;
	}
} // namespace

int main(int argc, char const* argv[])
{
	if (argc < 3)
	{
		std::fprintf(
			stderr,
			"EmbedAsset <inputFile> <symbolName> [namespaceName=\"\"] "
			"[-b|--base85]\n");
		return 1;
	}
	Settings settings{ argc, argv };

	std::ifstream inFile{ settings.m_AssetFilePath, std::ios::binary | std::ios::ate };
	if (!inFile.is_open())
	{
		std::fprintf(
			stderr,
			"Failed to open %s: %s\n",
			settings.m_AssetFilePath,
			std::strerror(errno));
		return 1;
	}
	std::vector<char> contents(inFile.tellg());
	inFile.seekg(0);
	if (!inFile.read(contents.data(), contents.size()))
	{
		std::fprintf(
			stderr,
			"Failed to read %s: %s\n",
			settings.m_AssetFilePath,
			std::strerror(errno));
		return 1;
	}
	if (contents.empty())
	{
		std::fprintf(stderr, "File is empty\n");
		return 1;
	}
	if (settings.m_Base85)
		return ExportBase85(settings, contents);
	return ExportArray(settings, contents);
}
