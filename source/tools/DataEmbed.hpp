#pragma once

#include <iostream>
#include <vector>

namespace brk::tools {
	bool EmbedData(
		std::ostream& output,
		std::istream& input,
		const char* assetName,
		const char* namespaceName = "brk");

	std::vector<char> ReadEmbeddedData(
		const char* data,
		uint32_t dataLen,
		uint32_t uncompressedSize);
} // namespace brk::tools
