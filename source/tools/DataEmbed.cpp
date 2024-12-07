#include "DataEmbed.hpp"

#include "Base85.hpp"

#include <core/Assert.hpp>
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

uint32 brk::tools::PackData(
	char* data,
	uint32 size,
	std::ostream& output,
	EPackFormat format)
{
	BRK_ASSERT(
		format > EPackFormat::Invalid && format < EPackFormat::NFormats,
		"Invalid packing format {}",
		int32(format));

	bz_stream bzStream = {};
	int errCode = BZ2_bzCompressInit(&bzStream, 9, 0, 0);
	DEBUG_CHECK(errCode == BZ_OK)
	{
		BRK_LOG_ERROR("Failed to init bz2 compression: {}", BzErrorString(errCode));
		return 0;
	}
	using TFlushFunction = uint32(const char*, uint32, std::ostream&);

	char buf[4096];
	uint32 nWritten = 0;
	bzStream.next_in = data;
	bzStream.avail_in = size;

	while (errCode != BZ_STREAM_END)
	{
		bzStream.next_out = buf;
		bzStream.avail_out = sizeof(buf);
		const uint32 pos = bzStream.total_out_lo32;
		errCode = BZ2_bzCompress(&bzStream, BZ_FINISH);
		if (errCode != BZ_FINISH_OK && errCode != BZ_STREAM_END)
		{
			BRK_LOG_ERROR("Compression failed: {}", BzErrorString(errCode));
			return false;
		}
		const uint32 diff = bzStream.total_out_lo32 - pos;
		if (!diff)
			continue;

		switch (format)
		{
		case EPackFormat::Raw:
			nWritten += diff;
			output.write(buf, diff);
			continue;
		case EPackFormat::Base85: nWritten += b85::Encode(buf, diff, output); continue;
		default: continue;
		}
	}

	BZ2_bzCompressEnd(&bzStream);
	return nWritten;
}

std::vector<char> brk::tools::UncompressData(
	char* data,
	uint32 dataLen,
	uint32 uncompressedSize)
{
	bz_stream stream = {};
	std::vector<char> uncompressed;
	uncompressed.reserve(uncompressedSize);

	BZ2_bzDecompressInit(&stream, 0, 0);
	stream.next_in = data;
	stream.avail_in = dataLen;

	int errCode = BZ_OK;
	char outBuf[2048];
	while (errCode != BZ_STREAM_END)
	{
		stream.next_out = outBuf;
		stream.avail_out = sizeof(outBuf);
		uint32 pos = stream.total_out_lo32;

		errCode = BZ2_bzDecompress(&stream);
		if (errCode != BZ_OK && errCode != BZ_STREAM_END)
		{
			BRK_LOG_ERROR("Decompression failed: {}", BzErrorString(errCode));
			return uncompressed;
		}
		const uint32 diff = stream.total_out_lo32 - pos;
		if (!diff)
			continue;
		uncompressed.resize(stream.total_out_lo32);
		std::memcpy(uncompressed.data() + pos, outBuf, diff);
	}
	BZ2_bzDecompressEnd(&stream);
	return uncompressed;
}

std::vector<char> brk::tools::DecodeEmbeddedData(StringView data, uint32 uncompressedSize)
{
	bz_stream stream = {};
	std::vector<char> uncompressed;
	uncompressed.reserve(uncompressedSize);

	BZ2_bzDecompressInit(&stream, 0, 0);

	int errCode = BZ_OK;
	char inBuf[1024], outBuf[2048];

	while (errCode != BZ_STREAM_END)
	{
		if (data.GetLen())
		{
			stream.avail_in = b85::Decode(data, inBuf, sizeof(inBuf));
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
