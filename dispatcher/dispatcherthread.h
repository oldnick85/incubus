#ifndef CDISPATCHERTHREAD_H
#define CDISPATCHERTHREAD_H

#include <condition_variable>
#include <atomic>
#include "dispatcher_common.h"
#include "dispatcheractor.h"

/**
 * @brief enDispatcherThreadAction - действия рабочего потока
 */
enum enDispatcherThreadAction
{
    taNone,         ///< пустое действие
    taOnStart,      ///< старт работы (первый колбек актора)
    taOnStop,       ///< остановка работы (последний колбек актора)
    taOnMsg,        ///< обработка сообщения
    taOnTimer,      ///< срабатывание таймера
    taSelfStop,     ///< становить рабочий поток
};

struct sDispatcherAction
{
    sDispatcherAction() {Reset();}

    void Reset()
    {
        action.store(taNone);
        //disp_actor.store(nullptr);
        tm_id.store(TIMER_ID_NONE);
    }

    sDispatcherAction(const sDispatcherAction &act)
    {
        action.store(act.action.load());
        //disp_actor.store(act.disp_actor.load());
        tm_id.store(act.tm_id.load());
    }

    void operator=(const sDispatcherAction &act)
    {
        action.store(act.action.load());
        //disp_actor.store(act.disp_actor.load());
        tm_id.store(act.tm_id.load());
    }

    std::string print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "act=%u", action.load());
        return s;
    }

    std::atomic<enDispatcherThreadAction>    action;
    std::atomic<TimerID_t>                   tm_id;
};

typedef uint32_t DispatcherThreadID_t;

enum
{
    DISPATCHER_THREAD_ID_NONE = static_cast<DispatcherThreadID_t>(-1),
};

/**
 * @brief CDispatcherThread - класс рабочего потока
 */
class CDispatcherThread
{
public:
    CDispatcherThread() = delete;

    CDispatcherThread(const DispatcherThreadID_t i);

    std::string print() const;

    bool Run();
    bool Stop();

    void SetActor(CDispatcherActorPtr actor);
    ActID_t GetActorId() const {return m_actor->Actor()->GetActorID().id;}
    void WakeUpForWork();
private:
    void WaitForWork();
    static void WorkingThread(CDispatcherThread* thr);

    std::string                             m_name;
    std::mutex                              m_mtx_wait_for_work;                  ///< мьютекс ожидания работы
    std::condition_variable                 m_cv_wait_for_work;                   ///< условие ожидания работы
    std::atomic<bool>                       m_should_stop;
    DispatcherThreadID_t                    id = DISPATCHER_THREAD_ID_NONE;     ///< идентификатор потока
    std::mutex                              m_mtx_actor;
    CDispatcherActorPtr                     m_actor;                            ///< текущий обрабатываемый актор
};

typedef std::shared_ptr<CDispatcherThread> CDispatcherThreadPtr;

#endif // CDISPATCHERTHREAD_H
