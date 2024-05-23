#pragma once

#include <PCH.hpp>
#include <memory>

namespace brk {
	/**
	 * Implements the singleton pattern for type T
	 */
	template <class T>
	class Singleton
	{
	public:
		/**
		 * Initializes the static instance using the provided arguments.
		 * If an instance had already been initialized, it is returned and no construction
		 * is performed \return If the singleton was already initialized, a reference to
		 * this instance is returned. Otherwise, returns a reference to the newly created
		 * instance.
		 */
		template <class... Args>
		static T& Init(Args&&... args);

		/**
		 * \return A reference to the static instance
		 * \attention If the singleton wasn't initialized, DebugBreak will be called
		 */
		static T& GetInstance();
		/**
		 * If the singleton was initialized, destroys the instance and sets the pointer to
		 * nullptr. Otherwise, does nothing.
		 */
		static void Reset();

	protected:
		Singleton() = default;

		static std::unique_ptr<T> s_Instance;
	};

} // namespace brk

#include "Singleton.inl"