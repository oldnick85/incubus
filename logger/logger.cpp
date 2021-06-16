#include <iostream>
//#include <sstream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <sys/time.h>
#include <thread>
#include "logger.h"

CLogger::CLogger()
{
    log_start_time = std::chrono::system_clock::now();
}

void CLogger::SetLogLevel(const enLogLevel level)
{
    m_max_level = level;
}

enLogLevel CLogger::GetLogLevel()
{
    return m_max_level;
}

void CLogger::LogMsg(const enLogLevel level, const std::string &logmessage)
{
    std::thread::id this_id = std::this_thread::get_id();
    if (level > m_max_level)
        return;
    std::lock_guard<std::mutex> locker1(m_mtx);
    uint64_t th = 0;
    memcpy(&th, &this_id, std::min(sizeof(th), sizeof(this_id)));
    uint16_t th_hash = 0;
    for (auto i = 0; i < 4; ++i)
        th_hash ^= static_cast<uint16_t>((th >> (i*16)) & 0xFFFF);

    struct timeval tv;
    gettimeofday(&tv, nullptr);
    std::string lvl_str;
    switch (level)
    {
    case llError:   lvl_str = "ERR";    break;
    case llWarning: lvl_str = "WRN";    break;
    case llDebug1:  lvl_str = "DB1";    break;
    case llDebug2:  lvl_str = "DB2";    break;
    case llDebug3:  lvl_str = "DB3";    break;
    default:                            break;
    }
    char s[256];
    auto ch_tm = std::chrono::system_clock::now() - log_start_time;
    uint64_t msc = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(ch_tm).count());

    snprintf(s, sizeof(s), "[%lu.%03lu][%04X][%s] %s\n",
             msc / 1000, msc % 1000, th_hash, lvl_str.c_str(), logmessage.c_str());
    std::cout << s;
}

void CLogger::LogMsgError(const std::string &logmessage)
{
    LogMsg(enLogLevel::llError, logmessage);
}

void CLogger::LogMsgWarning(const std::string &logmessage)
{
    LogMsg(enLogLevel::llWarning, logmessage);
}

void CLogger::LogMsgDebug1(const std::string &logmessage)
{
    LogMsg(enLogLevel::llDebug1, logmessage);
}

void CLogger::LogMsgDebug2(const std::string &logmessage)
{
    LogMsg(enLogLevel::llDebug2, logmessage);
}

void CLogger::LogMsgDebug3(const std::string &logmessage)
{
    LogMsg(enLogLevel::llDebug3, logmessage);
}
