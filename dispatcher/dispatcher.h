#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <atomic>
#include <unordered_map>
#include <map>
#include "i_dispatcher.h"
#include "../logger/i_logger.h"
#include "dispatcheractors.h"
#include "dispatcherthreads.h"

class CDispatcher
        : public IDispatcher
        , public CDispatcherActors
        , public CDispatcherThreads
{
public:
    CDispatcher(ILoggerPtr log);
    CDispatcher(const CDispatcher &other) = delete;

    virtual bool RegisterActor(IActor* actor) override final;
    virtual bool UnregisterActor(IActor* actor) override final;
    virtual bool SendMessage(const sMsg &message) override final;
    virtual void Run() override final;
    virtual void Stop() override final;

    virtual TimerID_t GetTimer(const ActID_t a_id, const enTimerType type = ttStrict) override final;
    virtual bool StartTimer(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms, const int_32 count) override final;
    virtual bool StartTimerOnce(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms) override final;
    virtual bool StartTimerPeriodical(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms) override final;
    virtual bool StopTimer(const ActID_t a_id, const TimerID_t timer_id) override final;
    virtual bool KillTimer(const ActID_t a_id, const TimerID_t timer_id) override final;
private:
    struct sDispCycleThreadsInfo
    {
        bool HasFreeThreads() const {return (free_threads_now != 0);}

        uint waiting_threads_before = 0;    // число ожидающих потоков в начале цикла
        uint free_threads_now = 0;          // число свободных потоков в конце цикла
        uint thr_count = 0;                 //
        uint working_threads_before = 0;    // число работающих потоков в начале цикла
    };
    /**
     * @brief DispatcherCycle - основной цикл диспетчера
     * @param dsp - объект диспетчера
     * @details В этом цикле происходит управление рабочими потоками, а также распределение
     *          ожидающих выполнения сообщений по рабочим потокам.
     */
    static void DispatcherCycle(CDispatcher *dsp);
    void ProcWorkthreads(sDispCycleThreadsInfo &thr_info);
    /**
     * @brief ThreadsLock - (за/раз)блокировать все структуры потоков
     * @param lock - признак блокировки/разблокировки
     */
    void ThreadsLock(const bool lock);
    bool CheckAllThreadsIsBusy() const;
    void LogDispatcherState(uint_64 i);
    /**
     * @brief StartThread - запустить рабочий поток для выполнения задания
     * @param thr - рабочий поток
     * @param disp_actor - назначенный для работы актор
     * @param action - назначенное действие
     */
    void StartThread(CDispatcherThread *thr, CDispatcherActor *disp_actor, const enDispatcherThreadAction action);

    std::chrono::time_point<std::chrono::high_resolution_clock> NearestTimeForTimer();
    std::string print_stat() const;

    std::atomic<bool>   m_waiting_for_workers;
    std::atomic<bool>   m_waiting_for_actions;
    std::atomic<bool>   m_should_stop_dispatcher;

    std::mutex              m_mtx_timers_wait;
    std::condition_variable m_cv_timers_wait;

    ILoggerPtr  m_log = nullptr;

    typedef std::multimap<std::chrono::time_point<std::chrono::high_resolution_clock>, TimerID_t> TimersOrder_t;

    struct sDispatcherStatistic
    {
        std::string print() const
        {
            char s[128];
            snprintf(s, sizeof(s), "[cycles all=%lu; idle=%lu; sleep_busy=%lu; msg sent=%lu; proc msg=%lu; start=%lu; timers=%lu]",
                     cycles, cycles_idle, sleep_busy, messages_sent, messages_proceed, onstart_proceed, timers_proceed);
            return s;
        }

        uint_64     cycles = 0;                 ///< количество циклов диспетчера
        uint_64     cycles_idle = 0;
        uint_64     sleep_busy = 0;
        uint_64     messages_sent = 0;
        uint_64     onstart_proceed = 0;
        uint_64     messages_proceed = 0;
        uint_64     timers_proceed = 0;
    };

    sDispatcherStatistic    m_stat;
};

typedef std::shared_ptr<CDispatcher>  CDispatcherPtr;

#endif // DISPATCHER_H
