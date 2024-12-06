#include "DataEmbed.hpp"

#include "Base85.hpp"

#include <core/LogManager.hpp>
#include <fstream>
#include <bzlib.h>

namespace {
	const char* BzErrorString(int code)
	{
		switch (code)
		{
		case BZ_OK: return "OK";
		case BZ_RUN_OK: return "BZ_RUN_OK";
		case BZ_FLUSH_OK: return "BZ_FLUSH_OK";
		case BZ_FINISH_OK: return "BZ_FINISH_OK";
		case BZ_STREAM_END: return "BZ_STREAM_END";
		case BZ_SEQUENCE_ERROR: return "BZ_SEQUENCE_ERROR";
		case BZ_PARAM_ERROR: return "BZ_PARAM_ERROR";
		case BZ_MEM_ERROR: return "BZ_MEM_ERROR";
		case BZ_DATA_ERROR: return "BZ_DATA_ERROR";
		case BZ_DATA_ERROR_MAGIC: return "BZ_DATA_ERROR_MAGIC";
		case BZ_IO_ERROR: return "BZ_IO_ERROR";
		case BZ_UNEXPECTED_EOF: return "BZ_UNEXPECTED_EOF";
		case BZ_OUTBUFF_FULL: return "BZ_OUTBUFF_FULL";
		case BZ_CONFIG_ERROR: return "BZ_CONFIG_ERROR";
		}
		return "Unknown";
	}
} // namespace

bool brk::tools::EmbedData(
	std::ostream& outStream,
	std::istream& file,
	const char* assetName,
	const char* namespaceName)
{
	file.seekg(0, std::ios::end);
	std::vector<char> fileContents(file.tellg());
	if (fileContents.empty())
	{
		BRK_LOG_ERROR("Couldn't embed {}: file is empty", assetName);
		return false;
	}

	file.seekg(0);
	file.read(fileContents.data(), fileContents.size());
	if (file.eof() || file.fail() || file.bad())
	{
		BRK_LOG_ERROR("Failed to read data: {}", std::strerror(errno));
		return false;
	}

	bz_stream bzStream = {};
	int errCode = BZ2_bzCompressInit(&bzStream, 9, 0, 0);
	DEBUG_CHECK(errCode == BZ_OK)
	{
		BRK_LOG_ERROR("Failed to init bz2 compression: {}", BzErrorString(errCode));
		return false;
	}

	outStream << "namespace " << namespaceName << R"( {
	constexpr struct {
		uint32 m_UncompressedSize;
		brk::StringView m_Data;
	} s_)" << assetName
			  << '{' << fileContents.size() << ",R\"~>(";

	bzStream.next_in = fileContents.data();
	bzStream.avail_in = uint32(fileContents.size());
	char buf[1024];
	bzStream.next_out = buf;
	bzStream.avail_out = sizeof(buf);

	while (errCode != BZ_STREAM_END)
	{
		errCode = BZ2_bzCompress(&bzStream, BZ_FINISH);
		if (errCode != BZ_FINISH_OK && errCode != BZ_STREAM_END)
		{
			BRK_LOG_ERROR("Compression failed: {}", BzErrorString(errCode));
			return false;
		}
		b85::Encode(buf, sizeof(buf) - bzStream.avail_out, outStream);
		bzStream.next_out = buf;
		bzStream.avail_out = sizeof(buf);
	}
	BZ2_bzCompressEnd(&bzStream);
	outStream << ")~>\"};\n}";

	return true;
}

std::vector<char> brk::tools::ReadEmbeddedData(
	const char* data,
	uint32 dataLen,
	uint32 uncompressedSize)
{
	bz_stream stream = {};
	std::vector<char> uncompressed;
	uncompressed.reserve(uncompressedSize);

	BZ2_bzDecompressInit(&stream, 0, 0);

	int errCode = BZ_OK;
	char inBuf[1024], outBuf[2048];

	StringView view{ data, dataLen };

	while (errCode != BZ_STREAM_END)
	{
		if (view.GetLen())
		{
			stream.avail_in = b85::Decode(view, inBuf, sizeof(inBuf));
			stream.next_in = inBuf;
		}

		stream.next_out = outBuf;
		stream.avail_out = sizeof(outBuf);
		uint32 pos = stream.total_out_lo32;

		errCode = BZ2_bzDecompress(&stream);
		if (errCode != BZ_OK && errCode != BZ_STREAM_END)
		{
			BRK_LOG_ERROR("Decompression failed: {}", BzErrorString(errCode));
			return uncompressed;
		}

		if (stream.total_out_lo32 == pos)
			continue;
		uncompressed.resize(stream.total_out_lo32);
		std::memcpy(uncompressed.data() + pos, outBuf, stream.total_out_lo32 - pos);
	}

	BZ2_bzDecompressEnd(&stream);
	return uncompressed;
}
