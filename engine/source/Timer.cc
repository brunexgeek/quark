#include <engine/Timer.hh>

#if (__cplusplus >= 201103L)

#include <chrono>
#include <atomic>

static std::chrono::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();
std::atomic<uint64_t> currentTime(Timer::getRealTime());

#else

uint64_t currentTime = Timer::getRealTime();

#ifdef __WINDOWS__
#include <Windows.h>
#endif

#if defined(__UNIX__) || defined(__ANDROID__)
#include <sys/time.h>
#endif

#endif


uint64_t Timer::getRealTime()
{
    #if (__cplusplus >= 201103L)
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count() / 1000;
	#else

    #if defined(__UNIX__) || defined(__ANDROID__)
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return  ((uint64_t) ts.tv_sec * 1000 * 1000) + (ts.tv_nsec / 1000);
    #endif

    #ifdef __WINDOWS__
    #error Implement this!
    #endif

    #endif
}


uint64_t Timer::getTime()
{
    return currentTime;
}


void Timer::update()
{
    currentTime = getRealTime();
}