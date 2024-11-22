#pragma once

#include <PCH.hpp>
#include <atomic>
#include "TypeTraits.hpp"

namespace brk {
	struct Retain_t
	{};
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

	using TRefCount = BasicRefCount<false>;
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
	 * \tparam Traits: The retain traits used to implement relevant operations on the
	 * reference counter: Increment, Decrement and GetCount
	 */
	template <class T, class Traits = RetainTraits<T>>
	class RetainPtr
	{
	public:
		using TraitsType = Traits;

		constexpr RetainPtr() = default;
		RetainPtr(T* ptr, Adopt_t);
		RetainPtr(T* ptr, Retain_t);
		explicit RetainPtr(T* ptr);

		RetainPtr(const RetainPtr& other);
		RetainPtr(RetainPtr&& other) noexcept;

		RetainPtr& operator=(const RetainPtr& other);
		RetainPtr& operator=(RetainPtr&& other) noexcept;

		~RetainPtr();

		void Reset(T* ptr, Adopt_t);
		void Reset(T* ptr, Retain_t);
		void Reset(T* ptr = nullptr);

		[[nodiscard]] T* Get() noexcept { return m_Ptr; }
		[[nodiscard]] const T* Get() const noexcept { return m_Ptr; }
		T* operator->() noexcept { return m_Ptr; }
		const T* operator->() const noexcept { return m_Ptr; }
		[[nodiscard]] T& operator*() { return *m_Ptr; }
		[[nodiscard]] const T& operator*() const { return *m_Ptr; }

		[[nodiscard]] constexpr operator bool() const noexcept { return bool(m_Ptr); }

		[[nodiscard]] auto GetCount() const;

		T* Release() noexcept { return std::exchange(m_Ptr, nullptr); }

	private:
		T* m_Ptr = nullptr;
	};
} // namespace brk

#include "RetainPtr.inl"