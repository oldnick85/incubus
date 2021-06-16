#ifndef I_DISPATCHER_H
#define I_DISPATCHER_H
#include <memory>
#include <iomanip>
#include <iostream>

#include "i_actor.h"
#include "dispatcher_common.h"
#include "description.h"

/**
 * @brief IDispatcher - интерфейс диспетчера, реализующего модель акторов
 */
class IDispatcher
{
public:
    /**
     * @brief enTimerType - тип таймера
     */
    enum enTimerType
    {
        ttStrict,       ///< точный таймер (может опоздать, но потом наверстает)
        ttSkipping,     ///< пропускающий таймер (может пропускать срабатывания, но срабатывает в точные моменты)
        ttContinuing,   ///< продолжающийся таймер (в случае задержки начинает отсчёт времени с момента срабатывания)
    };

    virtual ~IDispatcher() = default;
    virtual bool RegisterActor(IActor* actor) = 0;
    virtual bool UnregisterActor(IActor* actor) = 0;
    virtual bool SendMessage(const sMsg &message) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;

    virtual TimerID_t GetTimer(const ActID_t a_id, const enTimerType type = ttStrict) = 0;
    virtual bool StartTimer(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms, const int_32 count) = 0;
    virtual bool StartTimerOnce(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms) = 0;
    virtual bool StartTimerPeriodical(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms) = 0;
    virtual bool StopTimer(const ActID_t a_id, const TimerID_t timer_id) = 0;
    virtual bool KillTimer(const ActID_t a_id, const TimerID_t timer_id) = 0;
};
using IDispatcherPtr = std::shared_ptr<IDispatcher>;

#endif // I_DISPATCHER_H
