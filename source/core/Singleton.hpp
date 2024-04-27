#pragma once

#include <PCH.hpp>
#include <memory>

namespace brk {
	template <class T>
	class Singleton
	{
	public:
		template <class... Args>
		static T& Init(Args&&... args);

		static T& GetInstance();
		static void Reset();

	protected:
		Singleton() = default;

		static std::unique_ptr<T> s_Instance;
	};

} // namespace brk

#include "Singleton.inl"