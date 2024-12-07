#pragma once

#include <PCH.hpp>
#include "StringView.hpp"
#include <iostream>
#include <streambuf>
#include <vector>

namespace brk {
	class BRK_CORE_API InputByteBuf : public std::streambuf
	{
	public:
		InputByteBuf() = default;
		InputByteBuf(std::vector<char> buf)
			: m_Buf{ std::move(buf) }
		{}
		~InputByteBuf() = default;

		InputByteBuf(const InputByteBuf&) = delete;
		InputByteBuf(InputByteBuf&&) = default;
		InputByteBuf& operator=(const InputByteBuf&) = delete;
		InputByteBuf& operator=(InputByteBuf&&) = default;
		[[nodiscard]] const std::vector<char>& GetContents() const noexcept
		{
			return m_Buf;
		};

	protected:
		pos_type seekoff(
			off_type offset,
			std::ios_base::seekdir dir,
			std::ios_base::openmode) override;
		pos_type seekpos(pos_type pos, std::ios_base::openmode = std::ios_base::in)
			override;
		std::streamsize showmanyc() override;
		int_type underflow() override;
		std::streamsize xsgetn(char_type* s, std::streamsize count) override;

	private:
		std::vector<char> m_Buf;
		uint32 m_Pos = 0;
	};

	class BRK_CORE_API InputByteStream : public std::istream
	{
	public:
		InputByteStream()
			: std::istream(&m_Buf)
		{}
		InputByteStream(std::vector<char> data)
			: std::istream(&m_Buf)
			, m_Buf{ std::move(data) }
		{}
		[[nodiscard]] uint32 GetSize() const noexcept
		{
			return m_Buf.GetContents().size();
		}

	private:
		InputByteBuf m_Buf;
	};
} // namespace brk
