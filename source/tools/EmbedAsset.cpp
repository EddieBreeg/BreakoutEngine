#include "DataEmbed.hpp"
#include <fstream>

int main(int argc, char const* argv[])
{
	if (argc < 3)
	{
		std::fprintf(
			stderr,
			"EmbedAsset <inputFile> <symbolName> [namespaceName=\"\"]\n");
		return 1;
	}

	std::ifstream inFile{ argv[1], std::ios::binary | std::ios::ate };
	if (!inFile.is_open())
	{
		std::fprintf(stderr, "Failed to open %s: %s\n", argv[1], std::strerror(errno));
		return 1;
	}
	std::vector<char> contents(inFile.tellg());
	inFile.seekg(0);
	if (!inFile.read(contents.data(), contents.size()))
	{
		std::fprintf(stderr, "Failed to read %s: %s\n", argv[1], std::strerror(errno));
		return 1;
	}
	if (contents.empty())
	{
		std::fprintf(stderr, "File is empty\n");
		return 1;
	}

	const char* assetName = argv[2];
	const char* namespaceName = argc < 4 ? "" : argv[3];
	std::cout << "namespace " << namespaceName << R"({
	const struct { uint32 m_UncompressedSize; StringView m_Data; } s_)"
			<< assetName << "= {" << contents.size() << ", R\"~>(";

	if (!brk::tools::PackData(
			contents.data(),
			uint32(contents.size()),
			std::cout,
			brk::tools::EPackFormat::Base85))
		return 1;

	std::cout << ")~>\" };\n}";
	return 0;
}
