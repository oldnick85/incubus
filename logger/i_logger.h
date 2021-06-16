#ifndef I_LOGGER_H
#define I_LOGGER_H
#include <string>
#include <memory>

enum enLogLevel
{
    llNone,
    llError,
    llWarning,
    llDebug1,
    llDebug2,
    llDebug3,
};

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void SetLogLevel(const enLogLevel level) = 0;
    virtual enLogLevel GetLogLevel() = 0;
    virtual void LogMsg(const enLogLevel level, const std::string &logmessage) = 0;
    virtual void LogMsgError(const std::string &logmessage) = 0;
    virtual void LogMsgWarning(const std::string &logmessage) = 0;
    virtual void LogMsgDebug1(const std::string &logmessage) = 0;
    virtual void LogMsgDebug2(const std::string &logmessage) = 0;
    virtual void LogMsgDebug3(const std::string &logmessage) = 0;
};

typedef std::shared_ptr<ILogger> ILoggerPtr;

class CLogged
{
public:
    CLogged(ILoggerPtr log) : m_log(log) {}
    virtual ~CLogged() = default;
    ILoggerPtr Log() {return m_log;}
protected:
    ILoggerPtr m_log;
private:
};

#endif // I_LOGGER_H
