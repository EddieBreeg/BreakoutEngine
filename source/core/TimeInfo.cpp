#pragma once

#include "TimeInfo.hpp"

bre::TimeInfo::TimeInfo()
	: m_LastUpdate{ m_StartPoint }
{}

bre::TimeInfo::Duration bre::TimeInfo::Update()
{
	const TimePoint t = Clock::now();
	m_Delta = t - m_LastUpdate;
	m_LastUpdate = t;
	return m_Delta;
}

bre::TimeInfo::~TimeInfo() = default;