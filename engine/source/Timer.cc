#include <engine/Timer.hh>
#include <stdint.h>


#define ONE_SECOND    (1000 * 1000)


#if (__cplusplus >= 201103L)

#include <chrono>
#include <atomic>

static std::chrono::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();

#else


#ifdef __WINDOWS__
#include <Windows.h>
#endif

#if defined(__UNIX__) || defined(__ANDROID__)
#include <sys/time.h>
#endif

#endif

namespace quark {

// TODO: should be synchronized?
static uint64_t currentTime = 0;
static uint32_t currentFPS = 0;
static uint32_t frameCounter = 0;
static uint64_t timeCounter = 0;


uint64_t Timer::getRealTime()
{
    #if (__cplusplus >= 201103L)
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
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
    uint64_t fresh = getRealTime();

    timeCounter += fresh - currentTime;
    if (timeCounter > ONE_SECOND)
    {
        currentFPS = frameCounter;
        frameCounter = 0;
        timeCounter = 0;
    }

    ++frameCounter;
    currentTime = fresh;
}


uint32_t Timer::getFPS()
{
    return currentFPS;
}

}