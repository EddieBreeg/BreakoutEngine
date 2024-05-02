#include "ULID.hpp"
#include <array>

constexpr brk::ULID::ULID(
	const uint64 timestamp,
	const uint16 rand1,
	const uint64 rand2) noexcept
	: m_Left{ (timestamp << 16) | rand1 }
	, m_Right{ rand2 }
{}

template <uint32 N>
inline constexpr char* brk::ULID::ToChars(char (&out_buf)[N]) const noexcept
{
	static_assert(N >= 26, "Buffer is too small");
	constexpr char alphabet[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

	out_buf[0] = alphabet[(m_Left >> 61) & 31];
	out_buf[1] = alphabet[(m_Left >> 56) & 31];
	out_buf[2] = alphabet[(m_Left >> 51) & 31];
	out_buf[3] = alphabet[(m_Left >> 46) & 31];
	out_buf[4] = alphabet[(m_Left >> 41) & 31];
	out_buf[5] = alphabet[(m_Left >> 36) & 31];
	out_buf[6] = alphabet[(m_Left >> 31) & 31];
	out_buf[7] = alphabet[(m_Left >> 26) & 31];
	out_buf[8] = alphabet[(m_Left >> 21) & 31];
	out_buf[9] = alphabet[(m_Left >> 16) & 31];
	out_buf[10] = alphabet[(m_Left >> 11) & 31];
	out_buf[11] = alphabet[(m_Left >> 6) & 31];
	out_buf[12] = alphabet[(m_Left >> 1) & 31];
	out_buf[13] = alphabet[((m_Left << 4) | (m_Right >> 60)) & 31];
	out_buf[14] = alphabet[(m_Right >> 55) & 31];
	out_buf[15] = alphabet[(m_Right >> 50) & 31];
	out_buf[16] = alphabet[(m_Right >> 45) & 31];
	out_buf[17] = alphabet[(m_Right >> 40) & 31];
	out_buf[18] = alphabet[(m_Right >> 35) & 31];
	out_buf[19] = alphabet[(m_Right >> 30) & 31];
	out_buf[20] = alphabet[(m_Right >> 25) & 31];
	out_buf[21] = alphabet[(m_Right >> 20) & 31];
	out_buf[22] = alphabet[(m_Right >> 15) & 31];
	out_buf[23] = alphabet[(m_Right >> 10) & 31];
	out_buf[24] = alphabet[(m_Right >> 5) & 31];
	out_buf[25] = alphabet[m_Right & 31];

	return out_buf + 26;
}

constexpr brk::ULID brk::ULID::FromString(const std::string_view str) noexcept
{
	if (str.size() < 26)
		return {};
	constexpr uint8 map[] = {
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		0,	 1,	  2,   3,	4,	 5,	  6,   7,	8,	 9,	  255, 255, 255, 255, 255, 255,
		255, 10,  11,  12,	13,	 14,  15,  16,	17,	 255, 18,  19,	255, 20,  21,  255,
		22,	 23,  24,  25,	26,	 255, 27,  28,	29,	 30,  31,  255, 255, 255, 255, 255,
		255, 10,  11,  12,	13,	 14,  15,  16,	17,	 255, 18,  19,	255, 20,  21,  255,
		22,	 23,  24,  25,	26,	 255, 27,  28,	29,	 30,  31,  255, 255, 255, 255, 255
	};
	brk::ULID res;

	uint8 n = 0;
	for (uint8 i = 0; i < 13; ++i)
	{
		n = map[str[i]];
		if (n == 0xff)
			return {};
		res.m_Left = (res.m_Left << 5) | n;
	}
	n = map[str[13]];
	res.m_Left = (res.m_Left << 1) | (n >> 4);
	res.m_Right |= n & 0xff;

	for (uint8 i = 14; i < 26; i++)
	{
		n = map[str[i]];
		if (n == 0xff)
			return {};
		res.m_Right = (res.m_Right << 5) | n;
	}

	return res;
}

constexpr bool brk::ULID::operator==(const ULID other) const noexcept
{
	return m_Left == other.m_Left && m_Right == other.m_Right;
}

constexpr uint64 brk::Hash<brk::ULID>::operator()(const ULID id) const noexcept
{
	return CombineHash(id.m_Left, id.m_Right);
}
