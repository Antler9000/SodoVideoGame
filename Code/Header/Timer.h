#pragma once
#include <chrono>
using namespace std;
using namespace std::chrono;

class Timer
{
public:

	Timer()
	{
		m_startTime = m_clock.now();
		m_captionedTime = m_startTime;
		m_previousTime = m_startTime;
		m_currentTime = m_startTime;

		m_elapsedTime = m_currentTime - m_startTime;
		m_notCaptionedTime = m_currentTime - m_captionedTime;
		m_frameTime = m_currentTime - m_previousTime;
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

		m_elapsedTime = m_currentTime - m_startTime;
		m_notCaptionedTime = m_currentTime - m_captionedTime;
		m_frameTime = m_currentTime - m_previousTime;

		m_previousTime = m_currentTime;
	}

	void Captioned()
	{
		m_captionedTime = m_currentTime;

		m_notCaptionedTime = m_currentTime - m_captionedTime;
	}

	double GetEleapsedMilliSeconds()
	{
		return m_elapsedTime.count();
	}

	double GetNotCaptionedMilliSeconds()
	{
		return m_notCaptionedTime.count();
	}

	double GetFrameMilliSeconds()
	{
		return m_frameTime.count();
	}

private:

	steady_clock m_clock;

	time_point<steady_clock> m_startTime;
	time_point<steady_clock> m_captionedTime;
	time_point<steady_clock> m_previousTime;
	time_point<steady_clock> m_currentTime;

	duration<double, milli> m_elapsedTime;
	duration<double, milli> m_notCaptionedTime;
	duration<double, milli> m_frameTime;
};