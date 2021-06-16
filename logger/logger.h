#ifndef LOGGER_H
#define LOGGER_H
#include <mutex>
#include "i_logger.h"

class CLogger
        : public ILogger
{
public:
    CLogger();
    virtual void SetLogLevel(const enLogLevel level) override final;
    virtual enLogLevel GetLogLevel() override final;
    virtual void LogMsg(const enLogLevel level, const std::string &logmessage) override final;
    virtual void LogMsgError(const std::string &logmessage)  override final;
    virtual void LogMsgWarning(const std::string &logmessage)  override final;
    virtual void LogMsgDebug1(const std::string &logmessage)  override final;
    virtual void LogMsgDebug2(const std::string &logmessage)  override final;
    virtual void LogMsgDebug3(const std::string &logmessage)  override final;
private:
    std::mutex  m_mtx;
    std::chrono::time_point<std::chrono::system_clock>  log_start_time;

    enLogLevel m_max_level = llDebug3;
};

#endif // LOGGER_H
