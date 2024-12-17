#pragma once
#include <PCH.hpp>
#include "Assert.hpp"

namespace brk {
	struct BitsetView
	{
		BitsetView() = default;
		BitsetView(void* data, uint32 bitSize);
		BitsetView(const BitsetView&) = default;

		BitsetView& operator=(const BitsetView&) = default;

		[[nodiscard]] uint32 GetSize() const noexcept;
		[[nodiscard]] byte* GetPtr() noexcept;
		[[nodiscard]] const byte* GetPtr() const noexcept;

		[[nodiscard]] bool Get(uint32 index) const;
		[[nodiscard]] bool operator[](uint32 index) const;

		void Set(uint32 index);
		void Set(uint32 index, uint32 count);

		void Clear(uint32 index);
		void Clear(uint32 index, uint32 count);

		void SetAll();
		void ClearAll();

		static constexpr uint32 NPos = -1;

		[[nodiscard]] uint32 Find(bool val) const;
		[[nodiscard]] uint32 Find(bool val, uint32 count) const;

	private:
		byte* m_Data = nullptr;
		uint32 m_BitSize = 0;
	};

} // namespace brk

#include "Bitset.inl"