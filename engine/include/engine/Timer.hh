#ifndef ENGINE_TIMER_HH
#define ENGINE_TIMER_HH


#include <stdint.h>


#if defined(__WIN32__) || defined(__WIN64__) || defined(__WINNT__) || defined(__WINCE__) \
        || defined(__WIN16__) || defined(__CYGWIN__) || defined(WIN32) || defined(_WIN32) \
        || defined(_WIN64) || defined(WIN64)
        #define __WINDOWS__ 1
#endif
#if defined (ANDROID) || defined (__ANDROID__)
        #undef __ANDROID__
        #define __ANDROID__ 1
#endif
#if defined(__SOLARIS__) || defined(__QNXNTO__) || defined(__OPENBSD__) || defined(__NETBSD__) \
        || defined (__MACOS__) || defined(__MACOSX__) || defined (__LINUX__) || defined(__IRIX__) \
        || defined(__HPUX__) || defined(__FREEBSD__) || defined(__AIX__) || defined(__BDSI__) \
        || defined (__QNX__) || defined (__ANDROID__)
        #define __UNIX__    1
#endif



class Timer
{
    public:
        Timer() = delete;

        /**
         * Returns the current time.
         */
        static uint64_t getRealTime();

        /**
         * Returns the time computed when 'update' function was called.
         */
        static uint64_t getTime();

        /**
         * Update timing counter.
         */
        static void update();
};


#endif // ENGINE_TIMER_HH