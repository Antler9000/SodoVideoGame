#pragma once
#include <chrono>
#include <ratio>

class Timer
{
public:

	Timer() 
	{
		Reset();
	}

	~Timer() = default;

	//NOTE : 타이머 객체의 복사 또는 이동을 허용하지 않도록 함
	Timer(const Timer& sourceTimer) = delete;
	Timer(Timer&& sourceTimer) noexcept = delete;
	Timer& operator = (const Timer& sourceTimer) = delete;
	Timer& operator = (Timer&& sourceTimer) noexcept = delete;
	
	void Reset()
	{
		m_isStopped = false;

		m_startedTimePoint = Clock::now();
		m_stoppedTimePoint = m_startedTimePoint;
		m_currentTimePoint = m_startedTimePoint;

		m_pausedTime = Duration::zero();
		m_elapsedTime = Duration::zero();
	}

	void Stop()
	{
		if (m_isStopped == true)
		{
			return;
		}

		m_isStopped = true;

		m_stoppedTimePoint = Clock::now();
	}

	void Start()
	{
		if (m_isStopped == false)
		{
			return;
		}

		m_isStopped = false;

		m_pausedTime += Clock::now() - m_stoppedTimePoint;
	}

	void Mark()
	{
		m_startedTimePoint = Clock::now();
		m_stoppedTimePoint = m_startedTimePoint;
		m_currentTimePoint = m_startedTimePoint;

		m_pausedTime = Duration::zero();
	}

	void Update()
	{
		m_currentTimePoint = Clock::now();

		if (m_isStopped == false)
		{
			m_elapsedTime = (m_currentTimePoint - m_startedTimePoint) - m_pausedTime;
		}
		else
		{
			m_elapsedTime = (m_stoppedTimePoint - m_startedTimePoint) - m_pausedTime;
		}
	}

	double GetTimeMilli() const
	{
		return DurationMilli(m_elapsedTime).count();
	}

private:

	using Clock			= std::chrono::steady_clock;
	using TimePoint		= std::chrono::time_point<std::chrono::steady_clock>;
	using Duration		= std::chrono::steady_clock::duration;
	using DurationMilli	= std::chrono::duration<double, std::milli>;

	bool m_isStopped;

	TimePoint m_startedTimePoint;
	TimePoint m_stoppedTimePoint;
	TimePoint m_currentTimePoint;

	Duration m_pausedTime;
	Duration m_elapsedTime;
}; 