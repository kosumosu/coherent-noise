#pragma once

#include <chrono>

class HRTimer
{
public:
	HRTimer(void);
	~HRTimer(void);

	void Restart();

	float Sample() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

