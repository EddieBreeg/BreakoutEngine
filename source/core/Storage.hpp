#pragma once

#include <PCH.hpp>

namespace breakout {

	template <size_t Size, size_t Alignment = alignof(std::max_align_t)>
	struct Storage
	{
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
	auto CreateStorage(Args&&... args);

	template <class... T>
	using StorageFor = Storage<Max(sizeof(T)...),
							   Max(alignof(T)...)>;

} // namespace breakout

#include "Storage.inl"