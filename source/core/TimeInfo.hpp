#pragma once

#include <PCH.hpp>
#include <chrono>

namespace brk {
	class TimeInfo
	{
	public:
		using Duration = std::chrono::duration<float>;
		using Clock = std::chrono::steady_clock;
		using TimePoint = Clock::time_point;

		TimeInfo();
		Duration Update();
		~TimeInfo();

	private:
		TimePoint m_StartPoint = Clock::now();
		TimePoint m_LastUpdate;
		Duration m_Delta{ 0.0f };
		float m_Scale = 1.0f;
	};
} // namespace brk
