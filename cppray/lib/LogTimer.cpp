#include "cppray.h"

boost::posix_time::ptime LogTimer::GetTimeStamp()
{
    return boost::posix_time::microsec_clock::local_time();
}

LogTimer::LogTimer(const string &prefixMessage)
{
    _prefixMessage = prefixMessage;
    _startTime = GetTimeStamp();
}

LogTimer::~LogTimer()
{
    auto finish = GetTimeStamp();
    boost::posix_time::time_duration diff = finish - _startTime;
    double totalMs = diff.total_milliseconds();
    LogResult(totalMs);
}

void LogTimer::LogResult(double totalMilliseconds)
{
    cout << _prefixMessage << totalMilliseconds << "ms" << endl;
}