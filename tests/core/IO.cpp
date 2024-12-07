#include <core/IO.hpp>
#include <core/StringView.hpp>
#include <vector>

namespace {
	constexpr brk::StringView s_Str1 = "Foobar";
} // namespace

namespace brk::io::ut {
	void DynArrayBufTests()
	{
		{
			DynamicArrayStreamBuf streamBuf{ std::vector<char>{} };
			assert(streamBuf.in_avail() == -1);
		}
		{
			DynamicArrayStreamBuf streamBuf{ { s_Str1.begin(), s_Str1.end() } };
			assert(streamBuf.in_avail() == s_Str1.GetLen());
			assert(streamBuf.pubseekoff(1, std::ios_base::beg) == 1);
			assert(streamBuf.pubseekoff(1, std::ios_base::cur, std::ios::in) == 2);
			assert(streamBuf.pubseekoff(-1, std::ios_base::end) == (s_Str1.GetLen() - 1));

			char out[3];
			streamBuf.pubseekoff(0, std::ios_base::beg);
			assert(streamBuf.sgetc() == s_Str1[0]);
			assert(streamBuf.sgetn(out, sizeof(out)) == sizeof(out));
			assert(!std::strncmp(out, "oob", sizeof(out)));

			assert(streamBuf.pubseekpos(s_Str1.GetLen() - 1) == (s_Str1.GetLen() - 1));
			assert(streamBuf.sgetn(out, sizeof(out)) == 1);
			assert(streamBuf.sgetc() == DynamicArrayStreamBuf::traits_type::eof());
		}
		{
			DynamicArrayStreamBuf buf;
			buf.sputc('a');
			assert(buf.m_Buf.size() == 1);
			assert(buf.sgetc() == 'a');
			buf.pubseekpos(0);
			buf.sputn(s_Str1.GetPtr(), s_Str1.GetLen());

			char out[16] = { 0 };
			StringView str{ out, uint32(buf.sgetn(out, sizeof(out))) };
			assert(str == s_Str1);
		}
		{
			DynamicArrayStream inStream;
			assert(inStream.GetSize() == 0);
		}
		{
			DynamicArrayStream inStream{ { s_Str1.begin(), s_Str1.end() } };
			char out[s_Str1.GetLen()] = {};
			assert(inStream.readsome(out, sizeof(out)) == sizeof(out));
			assert(!inStream.eof());
			assert(inStream.get() == -1);
			assert(inStream.eof());
		}
		{
			DynamicArrayStream stream;
			stream.write(s_Str1.GetPtr(), s_Str1.GetLen());
			assert(stream.GetSize() == s_Str1.GetLen());
			char out[s_Str1.GetLen()] = {};
			const StringView view{ out, uint32(stream.readsome(out, sizeof(out))) };
			assert(view == s_Str1);
		}
	}
} // namespace brk::io::ut
