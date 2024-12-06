#include <tools/Base85.hpp>
#include <sstream>
#include <iostream>

namespace {
	void DoTest(uint32 len)
	{
		constexpr uint8 data[] = {
			0, 0, 0, 0, 0x73, 0x14, 0xd0, 0x8b,
		};
		std::stringstream stream;
		brk::b85::Encode(data, len, stream);
		const std::string str = stream.str();
		brk::StringView view{ str };
		std::cout << str << '\n';

		uint8 out[sizeof(data)] = {};

		uint32 nWritten = brk::b85::Decode(view, out, sizeof(out));
		assert(nWritten == len);
		for (uint32 i = 0; i < len; i++)
		{
			assert(out[i] == data[i]);
		}
	}
	constexpr const char s_Str1[] = "s8W-!EtluZ";
} // namespace

int main(int argc, char const* argv[])
{
	for (int32 i = 8; i; i--)
	{
		DoTest(i);
	}
	{
		brk::StringView view{ s_Str1, sizeof(s_Str1) - 1 };
		uint8 buf[7] = {}; // enough space for one block, but not two
		const uint32 n = brk::b85::Decode(view, buf, sizeof(buf));
		assert(n == 4);
		assert(buf[0] == 0xff);
		assert(buf[1] == 0xff);
		assert(buf[2] == 0xff);
		assert(buf[3] == 0xff);
		assert(buf[4] == 0);
		assert(buf[5] == 0);
		assert(buf[6] == 0);
	}
}