#pragma once

#include <PCH.hpp>

namespace brk {
	/** An aligned storage object
	 * \tparam Size: The size of the storage buffer
	 * \tparam Alignment: The alignment of the storage buffer
	 */
	template <size_t Size, size_t Alignment = alignof(std::max_align_t)>
	struct Storage
	{
		static_assert(
			Alignment && (Alignment & (~Alignment + 1)) == Alignment,
			"Alignment should be a power of 2");
		alignas(Alignment) uint8 m_Buf[Size];

		/**
		 * Constructs an object in the buffer, in place
		 * \tparam T: The type of object to create
		 * \param args: The arguments to be passed to the constructor
		 */
		template <class T, class... Args>
		Storage& Construct(Args&&... args);

		/**
		 * If T is trivially destructible, does nothing. Otherwise, calls T::~T() on the
		 * stored object.
		 * \warning If the storage doesn't in fact contain an object of T, the behaviour
		 * of this function is undefined
		 */
		template <class T>
		void DestroyAs() const;

		/** Returns the stored data, cast to type T
		 * \warning If the storage doesn't contain an object of type T, the data returned
		 * by this function is undefined
		 */
		template <class T>
		T& GetAs();

		/** Returns the stored data, cast to type T
		 * \warning If the storage doesn't contain an object of type T, the data returned
		 * by this function is undefined
		 */
		template <class T>
		const T& GetAs() const;
	};

	/** Utility function to create a storage object containing an object of type T */
	template <class T, class... Args>
	auto CreateStorage(Args&&... args) -> Storage<sizeof(T), alignof(T)>;

	/**
	 * Storage which meets the size and alignment requirements for all types in list T...
	 */
	template <class... T>
	using StorageFor = Storage<Max(sizeof(T)...), Max(alignof(T)...)>;

} // namespace brk

#include "Storage.inl"