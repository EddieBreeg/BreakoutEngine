#include <core/InputFile.hpp>
#include <core/StringView.hpp>
#include <vector>

namespace {
	constexpr brk::StringView s_Str1 = "Foobar";
} // namespace

namespace brk::io::ut {
	void InputFileTests()
	{
		{
			InputByteBuf streamBuf{ std::vector<char>{} };
			assert(streamBuf.in_avail() == -1);
		}
		{
			InputByteBuf streamBuf{ { s_Str1.begin(), s_Str1.end() } };
			assert(streamBuf.in_avail() == s_Str1.GetLen());
			assert(streamBuf.pubseekoff(1, std::ios_base::beg) == 1);
			assert(streamBuf.pubseekoff(1, std::ios_base::cur) == 2);
			assert(streamBuf.pubseekoff(-1, std::ios_base::end) == (s_Str1.GetLen() - 1));

			char out[3];
			streamBuf.pubseekoff(0, std::ios_base::beg);
			assert(streamBuf.sgetc() == s_Str1[0]);
			assert(streamBuf.sgetn(out, sizeof(out)) == sizeof(out));
			assert(!std::strncmp(out, "oob", sizeof(out)));

			assert(streamBuf.pubseekpos(s_Str1.GetLen() - 1) == (s_Str1.GetLen() - 1));
			assert(streamBuf.sgetn(out, sizeof(out)) == 1);
			assert(streamBuf.sgetc() == InputByteBuf::traits_type::eof());
		}
		{
			InputByteBuf streamBuf{ { s_Str1.begin(), s_Str1.end() } };
			std::istream inStream{ &streamBuf };
			char out[s_Str1.GetLen()] = {};
			assert(inStream.readsome(out, sizeof(out)) == sizeof(out));
			assert(!inStream.eof());
			assert(inStream.get() == -1);
			assert(inStream.eof());
		}
	}
} // namespace brk::io::ut
