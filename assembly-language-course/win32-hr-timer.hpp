#ifndef _LAPLAS_UTILS_H_
#define _LAPLAS_UTILS_H_

#include <Windows.h>

class HRTimer
{
public:
	HRTimer();
	double GetFrequency();
	void StartTimer();
	double StopTimer();
private:
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
	double frequency;
};

HRTimer::HRTimer(): start(), stop(),
frequency()
{
	frequency = this->GetFrequency();
}

double HRTimer::GetFrequency(void)
{
	LARGE_INTEGER proc_freq;
	QueryPerformanceFrequency(&proc_freq);
	return proc_freq.QuadPart;
}

void HRTimer::StartTimer(void)
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	::QueryPerformanceCounter(&start);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
}

double HRTimer::StopTimer(void)
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	::QueryPerformanceCounter(&stop);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	return ((stop.QuadPart - start.QuadPart) / frequency);
}

#endif	// _LAPLAS_UTILS_H_