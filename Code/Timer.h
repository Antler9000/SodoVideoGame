#pragma once
#include <chrono>

class Timer
{

public:

	Timer() :
		m_startTime(m_clock.now()),
		m_captionedTime(m_startTime),
		m_previousTime(m_startTime),
		m_currentTime(m_startTime),
		m_timeFromStart(m_currentTime - m_startTime),
		m_timeFromLastCaption(m_currentTime - m_captionedTime),
		m_timeFromLastFrame(m_currentTime - m_previousTime)
	{

	}

	~Timer()
	{

	}

	//NOTE : 타이머 객체의 복사 또는 이동을 허용하지 않도록 함
	Timer(const Timer& sourceTimer) = delete;
	Timer(Timer&& sourceTimer) noexcept = delete;
	Timer& operator = (const Timer& sourceTimer) = delete;
	Timer& operator = (Timer&& sourceTimer) noexcept = delete;

	void Update()
	{
		m_currentTime = m_clock.now();

		m_timeFromStart = m_currentTime - m_startTime;
		m_timeFromLastCaption = m_currentTime - m_captionedTime;
		m_timeFromLastFrame = m_currentTime - m_previousTime;

		m_previousTime = m_currentTime;
	}

	void MarkLastCaptionTime()
	{
		m_captionedTime = m_currentTime;

		m_timeFromLastCaption = m_currentTime - m_captionedTime;
	}

	double GetEleapsedMilliSeconds()
	{
		return m_timeFromStart.count();
	}

	double GetNotCaptionedMilliSeconds()
	{
		return m_timeFromLastCaption.count();
	}

	double GetFrameMilliSeconds()
	{
		return m_timeFromLastFrame.count();
	}

private:

	std::chrono::steady_clock m_clock;

	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	std::chrono::time_point<std::chrono::steady_clock> m_captionedTime;
	std::chrono::time_point<std::chrono::steady_clock> m_previousTime;
	std::chrono::time_point<std::chrono::steady_clock> m_currentTime;

	std::chrono::duration<double, std::milli> m_timeFromStart;
	std::chrono::duration<double, std::milli> m_timeFromLastCaption;
	std::chrono::duration<double, std::milli> m_timeFromLastFrame;
};