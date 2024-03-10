#pragma once

#include <PCH.hpp>
#include <core/MetaLists.hpp>

namespace breakout {

	template <size_t Size, size_t Alignment = alignof(std::max_align_t)>
	struct Storage
	{
		alignas(Alignment) uint8_t m_Buf[Size];

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
	using StorageFor = Storage<meta::ConstMax<size_t, sizeof(T)...>(),
							   meta::ConstMax<size_t, alignof(T)...>()>;

} // namespace breakout

#include "Storage.inl"