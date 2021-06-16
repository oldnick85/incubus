#ifndef CDISPATCHERTHREADS_H
#define CDISPATCHERTHREADS_H
#include "dispatcherthread.h"

class CDispatcherThreads
{
public:
    CDispatcherThreads();
protected:
    CDispatcherThreadPtr AddThread();
    bool DelThread(const DispatcherThreadID_t thr_id);

    void RunAllWorkers();
    void StopAllWorkers();

    CDispatcherThreadPtr FindThread(const DispatcherThreadID_t thr_id);
    CDispatcherThreadPtr FindThreadActor(const ActID_t act_id);
private:
    std::mutex              m_mtx_threads;
    std::unordered_map<DispatcherThreadID_t, CDispatcherThreadPtr> m_threads;
    uint_8                  m_max_threads = 1;              // максимальное число потоков для обработки акторов
    std::atomic<uint>       m_threads_count;
    DispatcherThreadID_t    m_max_thread_id = 0;
};

#endif // CDISPATCHERTHREADS_H
