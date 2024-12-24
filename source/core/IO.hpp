#pragma once

#include <PCH.hpp>
#include <iostream>
#include <streambuf>
#include <vector>

namespace brk {
	struct DynamicArrayStreamBuf : public std::streambuf
	{
		DynamicArrayStreamBuf() = default;
		BRK_CORE_API DynamicArrayStreamBuf(
			std::vector<char> buffer,
			uint64 readPos = 0,
			uint64 writePos = 0);

		std::vector<char> m_Buf;
		uint64 m_ReadPos = 0;
		uint64 m_WritePos = 0;

		static constexpr std::ios_base::openmode s_DefaultOpenMode =
			std::ios::in | std::ios::out;

	protected:
		BRK_CORE_API pos_type seekoff(
			off_type offset,
			std::ios_base::seekdir dir,
			std::ios_base::openmode mode = s_DefaultOpenMode) override;

		BRK_CORE_API pos_type
		seekpos(pos_type pos, std::ios_base::openmode = s_DefaultOpenMode) override;

		BRK_CORE_API std::streamsize showmanyc() override;
		BRK_CORE_API int_type underflow() override;
		BRK_CORE_API std::streamsize xsgetn(char_type* s, std::streamsize count) override;

		BRK_CORE_API std::streamsize xsputn(const char* data, std::streamsize n) override;
		BRK_CORE_API int_type overflow(int_type ch) override;
	};

	class DynamicArrayStream : public std::iostream
	{
	public:
		DynamicArrayStream()
			: std::iostream(&m_Buf)
		{}
		DynamicArrayStream(std::vector<char> data)
			: std::iostream(&m_Buf)
			, m_Buf{ std::move(data) }
		{}

		[[nodiscard]] uint32 GetSize() const noexcept
		{
			return static_cast<uint32>(m_Buf.m_Buf.size());
		}

		[[nodiscard]] DynamicArrayStreamBuf& GetBuffer() noexcept { return m_Buf; }
		[[nodiscard]] const DynamicArrayStreamBuf& GetBuffer() const noexcept
		{
			return m_Buf;
		}

	private:
		DynamicArrayStreamBuf m_Buf;
	};
} // namespace brk
