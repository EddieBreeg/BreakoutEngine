#pragma once

#include "TimeInfo.hpp"

breakout::TimeInfo::TimeInfo()
	: m_LastUpdate{ m_StartPoint }
{}

breakout::TimeInfo::Duration breakout::TimeInfo::Update()
{
	const TimePoint t = Clock::now();
	m_Delta = t - m_LastUpdate;
	m_LastUpdate = t;
	return m_Delta;
}

breakout::TimeInfo::~TimeInfo() = default;