#include "IO.hpp"
#include "Assert.hpp"
#include "DebugBreak.hpp"
#include "LogManager.hpp"

namespace brk {
	DynamicArrayStreamBuf::DynamicArrayStreamBuf(
		std::vector<char> buffer,
		uint64 readPos,
		uint64 writePos)
		: m_Buf{ std::move(buffer) }
		, m_ReadPos{ readPos }
		, m_WritePos{ writePos }
	{}

	std::streampos DynamicArrayStreamBuf::seekoff(
		off_type offset,
		std::ios_base::seekdir dir,
		std::ios_base::openmode mode)
	{
		switch (dir)
		{
		case std::ios::beg: return seekpos(offset, mode);
		case std::ios::end: return seekpos(m_Buf.size() + offset, mode);
		case std::ios::cur:
		{
			BRK_ASSERT(
				(mode & s_DefaultOpenMode) != s_DefaultOpenMode,
				"in+out open mode is not supported when stream direction is cur");
			if (mode & std::ios::in)
			{
				m_ReadPos += offset;
				BRK_ASSERT(
					m_ReadPos <= m_Buf.size(),
					"Tried to set read position to {} but current stream length is {}",
					m_ReadPos,
					m_Buf.size());
				return m_ReadPos;
			}
			if (mode & std::ios::out)
			{
				return m_WritePos += offset;
			}
			break;
		}
		default:
			BRK_LOG_CRITICAL("Invalid stream direction: {}", dir);
			dbg::Break();
			break;
		}
		return -1;
	}

	std::streampos DynamicArrayStreamBuf::seekpos(
		std::streampos pos,
		std::ios_base::openmode mode)
	{
		if (mode & std::ios::out)
			m_WritePos = pos;
		if (mode & std::ios::in)
		{
			BRK_ASSERT(
				pos <= m_Buf.size(),
				"Tried to set read position to {} but current stream length is {}",
				uint64(pos),
				m_Buf.size());
			m_ReadPos = pos;
		}
		return pos;
	}

	std::streamsize DynamicArrayStreamBuf::showmanyc()
	{
		if (m_ReadPos >= m_Buf.size())
			return -1;
		return m_Buf.size() - m_ReadPos;
	}

	int DynamicArrayStreamBuf::underflow()
	{
		if (m_ReadPos >= m_Buf.size())
			return traits_type::eof();
		int_type res = m_Buf[m_ReadPos++];

		return res;
	}

	std::streamsize DynamicArrayStreamBuf::xsgetn(char_type* s, std::streamsize count)
	{
		std::streamsize n = 0;
		while (n < count && m_ReadPos < m_Buf.size())
		{
			s[n++] = m_Buf[m_ReadPos++];
		}

		return n;
	}

	std::streamsize DynamicArrayStreamBuf::xsputn(const char* data, std::streamsize n)
	{
		if ((m_WritePos + n) > m_Buf.size())
		{
			m_Buf.resize(m_WritePos + n);
		}
		std::memcpy(m_Buf.data() + m_WritePos, data, n);
		m_WritePos += n;

		return n;
	}

	int DynamicArrayStreamBuf::overflow(int_type ch)
	{
		if (m_WritePos >= m_Buf.size())
			m_Buf.resize(m_WritePos + 1);
		m_Buf[m_WritePos++] = ch;
		return ch;
	}
} // namespace brk
