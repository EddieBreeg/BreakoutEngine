#include "InputFile.hpp"
#include "Assert.hpp"
#include "DebugBreak.hpp"
#include "LogManager.hpp"

namespace brk {
	InputByteBuf::pos_type InputByteBuf::seekoff(
		off_type offset,
		std::ios_base::seekdir dir,
		std::ios_base::openmode)
	{
		switch (dir)
		{
		case std::ios_base::beg:
			BRK_ASSERT(
				offset <= m_Buf.size(),
				"Attempted to seek offset {} in a stream of size {}",
				m_Pos,
				m_Buf.size());
			BRK_ASSERT(offset >= 0, "Attempted to seek negative offset: {}", offset);
			m_Pos = uint32(offset);
			break;
		case std::ios_base::cur:
			BRK_ASSERT(
				(m_Pos + offset) <= m_Buf.size(),
				"Attempted to seek offset {} in a stream of size {}",
				m_Pos + offset,
				m_Buf.size());
			m_Pos += offset;
			break;
		case std::ios_base::end:
			BRK_ASSERT(
				offset <= 0,
				"Attempted to seek offset {} in a stream of size {}",
				m_Buf.size() + offset,
				m_Buf.size());
			m_Pos = m_Buf.size() + offset;
			break;

		default:
			BRK_LOG_CRITICAL("Invalid seek direction type: {}", int64(dir));
			dbg::Break();
		}
		

		return m_Pos;
	}

	InputByteBuf::pos_type InputByteBuf::seekpos(pos_type pos, std::ios_base::openmode)
	{
		BRK_ASSERT(
			pos <= m_Buf.size(),
			"Attempted to seek offset {} in a stream of size {}",
			size_t(pos));
		
		return m_Pos = pos;
	}

	std::streamsize InputByteBuf::showmanyc()
	{
		if (m_Pos >= m_Buf.size())
			return -1;
		return m_Buf.size() - m_Pos;
	}

	InputByteBuf::int_type InputByteBuf::underflow()
	{
		if (m_Pos >= m_Buf.size())
			return traits_type::eof();
		int_type res = m_Buf[m_Pos++];
		
		return res;
	}

	std::streamsize InputByteBuf::xsgetn(char_type* s, std::streamsize count)
	{
		std::streamsize n = 0;
		while (n < count && m_Pos < m_Buf.size())
		{
			s[n++] = m_Buf[m_Pos++];
		}
		
		return n;
	}
} // namespace brk
