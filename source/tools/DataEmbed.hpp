#pragma once

#include <PCH.hpp>
#include <core/StringView.hpp>
#include <iostream>
#include <vector>

namespace brk::tools {
	enum class EPackFormat : int8
	{
		Invalid = -1,
		Raw,	// No special encoding is used, the compressed data is written as is
		Base85, // The data is encoded with b85::Encode before writing
		NFormats
	};

	/**
	 * Compressed the provided data, writes it into the output stream using the specified
	 * format.
	 * \param data: The data to be packed
	 * \param size: The size of the data
	 * \param output: The stream to write into
	 * \param format: The format used to encode the data. See EPackFormat for details
	 * \returns How many bytes were written into the output, or 0 if an error occurred
	 */
	uint32 PackData(
		char* data,
		uint32 size,
		std::ostream& output,
		EPackFormat format = EPackFormat::Raw);

	/**
	 * Uncompresses raw data. This assumes said data was first compressed with bz2, e.g.
	 * using PackData with the Raw format.
	 * \param data: The data to decompress
	 * \param size: The data size
	 * \param uncompressedSize: The size of the data in uncompressed form. This is used to
	 * reserve an adequate amount of storage upfront, thus avoiding unnecessary
	 * reallocations. If unknown, 0 may be used instead.
	 */
	std::vector<char> UncompressData(char* data, uint32 size, uint32 uncompressedSize);

	/**
	 * Uncompresses data which was encoded with b85::Encode.
	 * \param data: The base85 string
	 * \param uncompressedSize: The size of the data in uncompressed form. This is used to
	 * reserve an adequate amount of storage upfront, thus avoiding unnecessary
	 * reallocations. If unknown, 0 may be used instead.
	 */
	std::vector<char> DecodeEmbeddedData(StringView data, uint32 uncompressedSize);
} // namespace brk::tools
