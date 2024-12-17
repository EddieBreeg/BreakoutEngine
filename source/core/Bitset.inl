#include "Bitset.hpp"
#define _LROT1(byte) (((byte) << 1) | ((byte) >> 7))

namespace brk {
	inline BitsetView::BitsetView(void* ptr, uint32 len)
		: m_Data{ static_cast<byte*>(ptr) }
		, m_BitSize{ len }
	{}

	inline uint32 brk::BitsetView::GetSize() const noexcept
	{
		return m_BitSize;
	}

	inline byte* brk::BitsetView::GetPtr() noexcept
	{
		return m_Data;
	}

	inline const byte* BitsetView::GetPtr() const noexcept
	{
		return m_Data;
	}

	inline bool BitsetView::Get(uint32 index) const
	{
		BRK_ASSERT(index < m_BitSize, "Index {} is out of range", index);
		return m_Data[index / 8] & BIT(index % 8);
	}

	inline bool BitsetView::operator[](uint32 index) const
	{
		return Get(index);
	}

	inline void BitsetView::Set(uint32 index)
	{
		BRK_ASSERT(index < m_BitSize, "Index {} is out of range", index);
		m_Data[index / 8] |= BIT(index % 8);
	}

	inline void BitsetView::Set(uint32 index, uint32 count)
	{
		BRK_ASSERT(
			(index + count) <= m_BitSize,
			"Index {} is out of range",
			index + count - 1);
		byte bit = BIT(index % 8);
		while (count--)
		{
			m_Data[index++ / 8] |= bit;
			bit = _LROT1(bit);
		}
	}

	inline void BitsetView::Clear(uint32 index)
	{
		BRK_ASSERT(index < m_BitSize, "Index {} is out of range", index);
		m_Data[index / 8] &= ~BIT(index % 8);
	}

	inline void BitsetView::Clear(uint32 index, uint32 count)
	{
		BRK_ASSERT(
			(index + count) <= m_BitSize,
			"Index {} is out of range",
			index + count - 1);
		byte bit = BIT(index % 8);
		while (count--)
		{
			m_Data[index++ / 8] &= ~bit;
			bit = _LROT1(bit);
		}
	}

	inline void BitsetView::SetAll()
	{
		uint32 count = (m_BitSize - 1) / 8 + 1;
		while (count--)
			m_Data[count] = 0xff;
	}

	inline void BitsetView::ClearAll()
	{
		uint32 count = (m_BitSize - 1) / 8 + 1;
		while (count--)
			m_Data[count] = 0;
	}

	inline uint32 BitsetView::Find(bool val) const
	{
		byte bit = BIT(0);
		for (uint32 i = 0; i < m_BitSize; i++)
		{
			if (bool(m_Data[i / 8] & bit) == val)
				return i;
			bit = _LROT1(bit);
		}

		return NPos;
	}

	inline uint32 BitsetView::Find(bool val, uint32 count) const
	{
		if (count > m_BitSize)
			return NPos;

		byte bit = BIT(0);
		uint32 sequenceLen = 0;
		uint32 pos = 0;

		while ((pos + count) <= m_BitSize)
		{
			const uint32 index = pos + sequenceLen++;
			if (bool(m_Data[index / 8] & bit) != val)
			{
				pos += sequenceLen;
				sequenceLen = 0;
				goto LOOP_END;
			}

			if (sequenceLen == count)
				return pos;

		LOOP_END:
			bit = _LROT1(bit);
		}
		return NPos;
	}
} // namespace brk

#undef _LROT1