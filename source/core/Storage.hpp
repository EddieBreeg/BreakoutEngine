#pragma once

#include <PCH.hpp>

namespace brk {

	template <size_t Size, size_t Alignment = alignof(std::max_align_t)>
	struct Storage
	{
		static_assert(
			Alignment && (Alignment & (~Alignment + 1)) == Alignment,
			"Alignment should be a power of 2");
		alignas(Alignment) uint8 m_Buf[Size];

		template <class T, class... Args>
		Storage& Construct(Args&&... args);

		template <class T>
		void DestroyAs() const;

		template <class T>
		T& GetAs();

		template <class T>
		const T& GetAs() const;
	};

	template <class T, class... Args>
	auto CreateStorage(Args&&... args) -> Storage<sizeof(T), alignof(T)>;

	/**
	 * Storage which meets the size and alignment requirements for all types in T
	 */
	template <class... T>
	using StorageFor = Storage<Max(sizeof(T)...), Max(alignof(T)...)>;

} // namespace brk

#include "Storage.inl"