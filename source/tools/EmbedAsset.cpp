#include "DataEmbed.hpp"
#include <fstream>

int main(int argc, char const* argv[])
{
	if (argc < 4)
	{
		std::fprintf(
			stderr,
			"EmbedAsset <inputFile> <outputFile> <symbolName> [namespaceName=brk]\n");
		return 1;
	}

	std::ifstream inFile{ argv[1], std::ios::binary | std::ios::ate };
	if (!inFile.is_open())
	{
		std::fprintf(stderr, "Failed to open %s: %s\n", argv[1], std::strerror(errno));
		return 1;
	}

	std::ofstream outFile{ argv[2], std::ios::binary };
	if (!inFile.is_open())
	{
		std::fprintf(
			stderr,
			"Failed to open %s for writing: %s\n",
			argv[2],
			std::strerror(errno));
		return 1;
	}
	brk::tools::EmbedData(outFile, inFile, argv[3], argc < 5 ? "brk" : argv[4]);
	return 0;
}
