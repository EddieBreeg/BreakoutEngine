#pragma once

#include <PCH.hpp>
#include <iostream>
#include <core/Assert.hpp>
#include <core/StringView.hpp>

namespace brk::b85 {
	/**
	 * Encodes arbitrary data into base85
	 * \param input: The data to be encoded
	 * \param size: How many bytes should be encoded
	 * \param output: The stream to write into
	 * \returns The number of characters that were written to the stream
	 * \warning This function does not support the z exception. Indeed, it is meant to be
	 * used on compressed data, so there's no point in making decoding harder for an
	 * exception that will most likely never occur anyway
	 */
	uint32 Encode(const void* input, uint32 size, std::ostream& output)
	{
		uint32 written = 0;
		const uint8* pos = static_cast<const uint8*>(input);
		constexpr uint32 fac[] = { 85 * 85 * 85 * 85, 85 * 85 * 85, 85 * 85, 85 };
		while (size >= 4)
		{
			uint32 block = (uint32(pos[0]) << 24) | (uint32(pos[1]) << 16) |
						   (uint32(pos[2]) << 8) | uint32(pos[3]);
			pos += 4;
			size -= 4;

			output << char((block / fac[0]) + 33);
			output << char((block / fac[1] % 85) + 33);
			output << char((block / fac[2] % 85) + 33);
			output << char((block / fac[3] % 85) + 33);
			output << char((block % 85) + 33);
			written += 5;
		}
		if (!size)
			return written;

		uint32 block = 0;
		for (uint32 i = 0; i < size; i++)
			block |= uint32(pos[i]) << 8 * (3 - i);
		output << char(block / fac[0] + 33);

		for (uint32 i = 0; i < size; i++)
			output << char((block / fac[i + 1] % 85) + 33);
		return written + size + 1;
	}

	/**
	 * Decodes a base85 string, and writes the output data in the provided buffer.
	 * \param text: The text to decode. All consumed characters will be trimmed from the
	 * view. In other words, after calling Decode, text only contains the characters which
	 * haven't been decoded yet.
	 * \param out: The output buffer
	 * \param bufSize: The available space in the output buffer
	 * \returns The number of bytes written to the output buffer
	 */
	uint32 Decode(StringView& text, void* out, uint32 bufSize)
	{
		BRK_ASSERT(
			(text.GetLen() % 5 == 0) || (text.GetLen() % 5) > 1,
			"Base85 string has invalid length {}",
			text.GetLen());

		constexpr uint32 fac[] = { 85 * 85 * 85 * 85, 85 * 85 * 85, 85 * 85, 85, 1 };
		uint32 pos = 0;
		uint8* buf = static_cast<uint8*>(out);

		while (text.GetLen() > 4 && (bufSize - pos) > 3)
		{
			const uint32 block = fac[0] * uint32(text[0] - 33) +
								 fac[1] * uint32(text[1] - 33) +
								 fac[2] * uint32(text[2] - 33) +
								 fac[3] * uint32(text[3] - 33) + uint32(text[4] - 33);
			buf[pos++] = uint8(block >> 24);
			buf[pos++] = uint8(block >> 16);
			buf[pos++] = uint8(block >> 8);
			buf[pos++] = uint8(block);
			text.TrimLeft(5);
		}

		uint32 blockSize = text.GetLen();
		if (!blockSize || (pos + --blockSize) > bufSize)
			return pos;

		uint32 block = 0;
		for (uint32 i = 0; i <= blockSize; i++)
			block += fac[i] * uint32(text[i] - 33);
		for (uint32 i = blockSize + 1; i < 5; ++i)
			block += fac[i] * 84;
		for (uint32 i = 0; i < blockSize; i++)
			buf[pos++] = uint8(block >> 8 * (3 - i));

		return pos;
	}
} // namespace brk::b85
