#pragma once

#include <PCH.hpp>
#include "Assert.hpp"
#include "TypeId.hpp"

namespace brk {
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
				std::is_same_v<std::invoke_result_t<F, Args...>, R>,
			int>;

	public:
		UniqueFunction() noexcept = default;
		UniqueFunction(const UniqueFunction&) = delete;
		explicit UniqueFunction(UniqueFunction&&);
		~UniqueFunction();

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

		R operator()(Args... args);

		void Reset() noexcept;

		[[nodiscard]] operator bool() const noexcept { return m_Table; }
		[[nodiscard]] const TypeId& GetId() const noexcept;

	private:
		void* GetPtr() noexcept;
		union {
			void* m_Ptr = nullptr;
			uint8 m_Buf[sizeof(m_Ptr)];
		};
		const VTable* m_Table = nullptr;
	};
} // namespace brk

#include "Function.inl"