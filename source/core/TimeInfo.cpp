#pragma once

#include "TimeInfo.hpp"

brk::TimeInfo::TimeInfo()
	: m_LastUpdate{ m_StartPoint }
{}

brk::TimeInfo::Duration brk::TimeInfo::Update()
{
	const TimePoint t = Clock::now();
	m_Delta = t - m_LastUpdate;
	m_LastUpdate = t;
	return m_Delta;
}

brk::TimeInfo::~TimeInfo() = default;