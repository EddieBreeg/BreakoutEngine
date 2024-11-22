#pragma once

#include <PCH.hpp>
#include "Assert.hpp"
#include "TypeId.hpp"

namespace brk {
	/** Represents a non copyable callable object of signature F */
	template <class F>
	class UniqueFunction;

	template <class R, class... Args>
	class UniqueFunction<R(Args...)>
	{
		struct VTable
		{
			const TypeId& m_Id;
			void (*m_Destroy)(void*) = nullptr;
			void (*m_MoveAssign)(void*, void*) = nullptr;
			R (*m_Invoke)(void* ptr, Args... args) = nullptr;

			template <class F>
			static VTable Implement();

			template <class F>
			static const VTable s_Instance;
		};
		template <class F>
		using IfSuitable = std::enable_if_t<
			!std::is_same_v<std::decay_t<F>, UniqueFunction> &&
				std::is_convertible_v<std::invoke_result_t<F, Args...>, R>,
			int>;

	public:
		/** Constructs an empty function object */
		UniqueFunction() noexcept = default;
		UniqueFunction(const UniqueFunction&) = delete;
		explicit UniqueFunction(UniqueFunction&&);
		~UniqueFunction();

		/**
		 * Constructs the function object with a suitable object.
		 * This overload is picked if std::decay_t<F> is not UniqueFunction,
		 * F behaves as a callable object which can take arguments of types
		 * Args... and returns an object of type R.
		 * \param func: The callable object which will be stored.
		 * \note If sizeof(func) <= sizeof(void*), small object optimization is used:
		 * the UniqueFunction object is guaranteed not to use any dynamic allocation
		 */
		template <class F, IfSuitable<F> = 0>
		UniqueFunction(F&& func)
			: m_Table{ &VTable::s_Instance<std::decay_t<F>> }
		{
			using TRaw = std::decay_t<F>;
			if constexpr (sizeof(TRaw) <= sizeof(m_Buf))
			{
				new (m_Buf) TRaw{ std::forward<F>(func) };
			}
			else
			{
				m_Ptr = new TRaw{ std::forward<F>(func) };
			}
		}

		UniqueFunction& operator=(UniqueFunction&& other) noexcept;

		/**
		 * Assignment from a generic callable object.
		 * \param other: A callable object. Must not be an object of type UniqueFunction.
		 * Must behave as a function of type R(Args...)
		 */
		template <class F, IfSuitable<F> = 0>
		UniqueFunction& operator=(F&& other)
		{
			using TRaw = std::decay_t<F>;
			const VTable* table = &VTable::s_Instance<TRaw>;
			if (std::is_move_assignable_v<TRaw> && table == m_Table)
			{
				m_Table->m_MoveAssign(GetPtr(), &other);
				return *this;
			}
			Reset();
			new (this) UniqueFunction(std::forward<F>(other));
			return *this;
		}

		/**
		 * Call operator. If a valid callable object is stored, it is called and
		 * the result is returned. Otherwise, this function asserts.
		 */
		R operator()(Args... args) const;

		/**
		 * Destroyes the stored object. After a call to this function, the bool operator
		 * returns false
		 */
		void Reset() noexcept;

		/**
		 * Returns true if the UniqueFunction object contains a valid callable,
		 * false otherwise.
		 */
		[[nodiscard]] operator bool() const noexcept { return m_Table; }
		[[nodiscard]] const TypeId& GetId() const noexcept;

	private:
		void* GetPtr() noexcept;
		void* GetPtr() const noexcept;
		union {
			void* m_Ptr = nullptr;
			uint8 m_Buf[sizeof(m_Ptr)];
		};
		const VTable* m_Table = nullptr;
	};
} // namespace brk

#include "Function.inl"