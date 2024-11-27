#pragma once

#include <PCH.hpp>
#include <chrono>

namespace brk {
	/**
	 * A utility used to track time
	 */
	class TimeInfo
	{
	public:
		using Duration = std::chrono::duration<float>;
		using Clock = std::chrono::steady_clock;
		using TimePoint = Clock::time_point;

		/** Initializes the start point to Clock::now() */
		TimeInfo();
		/** Sets the LastUpdate point, and computes the time delta since the last call to
		 * this function */
		Duration Update();
		Duration GetElapsed() const noexcept { return m_LastUpdate - m_StartPoint; }
		~TimeInfo();

	private:
		TimePoint m_StartPoint = Clock::now();
		TimePoint m_LastUpdate;
		Duration m_Delta{ 0.0f };
		float m_Scale = 1.0f;
	};
} // namespace brk
