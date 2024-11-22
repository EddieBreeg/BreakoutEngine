#pragma once

#include <PCH.hpp>
#include <atomic>
#include "TypeTraits.hpp"

namespace brk {
	/**
	 * Retain action: specifies the reference count should be increment when storing a
	 * new pointer
	 */
	struct Retain_t
	{};
	/**
	 * Retain action: specifies the reference count should be left alone when storing
	 * new pointer
	 */
	struct Adopt_t
	{};

	static constexpr Retain_t s_Retain;
	static constexpr Adopt_t s_Adopt;

	/**
	 * Trait class which implements the necessary APIs for a given ref counter class
	 * \tparam T: The ref counted class
	 * \details A valid retain traits class must have at least 3 static functions:
	 * Increment, Decrement and GetCount. Additionally, it can also provide a static
	 * DefaultAction value, to specify which should be the default behaviour when creating
	 * a new RetainPtr. If not specified, s_Adopt will be used.
	 * It must also declare a public static const value named DefaultAction of type either
	 * Retain_t or Adopt_t. This value specifies the default behaviour when storing a new
	 * pointer in a RetainPtr object
	 */
	template <class T>
	struct RetainTraits;

	/**
	 * Very simple reference counter
	 */
	template <bool Atomic>
	struct BasicRefCount
	{
		using ValueType = std::conditional_t<Atomic, std::atomic<uint32>, uint32>;
		template <class T>
		friend struct RetainTraits;

		BasicRefCount() = default;

	protected:
		ValueType m_Count = 1;
	};

	/* Simple non-atomic ref counter */
	using TRefCount = BasicRefCount<false>;
	/* Simple atomic ref counter */
	using TAtomicRefCount = BasicRefCount<true>;

	namespace _internal {
		template <class T, class = void>
		struct RefCountBase;

		template <class T>
		struct RefCountBase<
			T,
			std::enable_if_t<std::is_base_of_v<BasicRefCount<true>, T>>>
		{
			using BaseType = BasicRefCount<true>;
		};
		template <class T>
		struct RefCountBase<
			T,
			std::enable_if_t<std::is_base_of_v<BasicRefCount<false>, T>>>
		{
			using BaseType = BasicRefCount<false>;
		};
	}; // namespace _internal

	// Default retain traits, for classes which inherit from a BasicRefCount type
	template <class T>
	struct RetainTraits
	{
		static_assert(
			meta::IsComplete<_internal::RefCountBase<T>>,
			"Invalid ref counted type; consider inheriting from BasicRefCount or "
			"specializing RetainTraits");

		static constexpr Adopt_t DefaultAction = {};
		// Increments the internal ref counter
		static void Increment(T* ref);

		// Decrements the internal ref counter, and calls delete if it reaches 0
		static void Decrement(T* ref);

		// Returns the internal ref counter
		static uint32 GetCount(const T* ref);
	};

	/**
	 *  A smart pointer with intrusive reference counting
	 * \tparam T: The type of object pointed at
	 * \tparam Traits: The retain traits which defines all relevant operations on the
	 * reference counter (Increment, Decrement and GetCount) as well as DefaultAction
	 */
	template <class T, class Traits = RetainTraits<T>>
	class RetainPtr
	{
	public:
		using TraitsType = Traits;

		/**
		 * Initializes the stored pointer to nullptr
		 */
		constexpr RetainPtr() = default;
		/**
		 * Constructs an object which stores the specified pointer, without increment
		 * the reference count
		 */
		RetainPtr(T* ptr, Adopt_t);
		/**
		 * Stores the specified pointer, and increments its reference count if ptr is not
		 * nullptr
		 */
		RetainPtr(T* ptr, Retain_t);
		/**
		 * Stores the specified pointer, and derives the ref count behaviour from
		 * Traits::DefaultAction
		 */
		explicit RetainPtr(T* ptr);

		/** Copies the stored pointer and increments the ref count if not null */
		RetainPtr(const RetainPtr& other);
		RetainPtr(RetainPtr&& other) noexcept;

		/** Effectively calls Reset(ptr, other.ptr) */
		RetainPtr& operator=(const RetainPtr& other);
		/** Effectively calls std::swap(ptr, other.ptr) */
		RetainPtr& operator=(RetainPtr&& other) noexcept;

		/** If the stored pointer is not null, decrements the reference counter */
		~RetainPtr();

		/** Replaces the stored pointer with the specified value, without incrementing the
		 * reference counter. If the previously stored pointer was not nullptr, the
		 * reference count is decremented */
		void Reset(T* ptr, Adopt_t);
		/** Replaces the stored pointer with the specified value, and increments the
		 * reference counter if said value is not nullptr. . If the previously stored
		 * pointer was not nullptr, the reference count is decremented */
		void Reset(T* ptr, Retain_t);
		/** Calls Reset(ptr, Traits::DefaultAction) */
		void Reset(T* ptr = nullptr);

		[[nodiscard]] T* Get() noexcept { return m_Ptr; }
		[[nodiscard]] const T* Get() const noexcept { return m_Ptr; }
		T* operator->() noexcept { return m_Ptr; }
		const T* operator->() const noexcept { return m_Ptr; }
		[[nodiscard]] T& operator*() { return *m_Ptr; }
		[[nodiscard]] const T& operator*() const { return *m_Ptr; }

		[[nodiscard]] constexpr operator bool() const noexcept { return bool(m_Ptr); }

		/** Returns the current value of the reference counter. Only thread safe is the
		 * counter itself is atomic */
		[[nodiscard]] auto GetCount() const;

		/** Replaces the stored pointer with nullptr without decrementing the reference
		 * count, and returns the old value */
		T* Release() noexcept { return std::exchange(m_Ptr, nullptr); }

	private:
		T* m_Ptr = nullptr;
	};
} // namespace brk

#include "RetainPtr.inl"