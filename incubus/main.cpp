#include <iostream>
#include <thread>
#include <unordered_map>
#include <time.h>
#include <memory>
#include "incubus_common.h"
#include "description.h"
#include "../dispatcher/actor.h"
#include "../dispatcher/dispatcher.h"
#include "cores/gameplay.h"
#include "cores/physics.h"
#include "cores/behavior.h"
#include "cores/show.h"
#include "../logger/logger.h"
#include "test/unittest.h"

using namespace std;

#undef TEST_ON
//#define TEST_ON

int main()
{
    srand(static_cast<uint>(time(nullptr)));
    ILoggerPtr m_log(new CLogger);

    log_warning("Start Incubus %u.%u.%u (%s)",
                VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, __TIMESTAMP__);

#ifdef TEST_ON
    CUnitTest ut(m_log);
    if (not ut.TestCollisionMovingPoint())
        return 1;
#endif

    IDispatcherPtr disp(new CDispatcher(m_log));
    std::shared_ptr<CGameplay> gp(new CGameplay(m_log, disp));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    log_debug1("Dispatcher start");
    m_log->SetLogLevel(llError);
    disp->Run();
    m_log->SetLogLevel(llDebug3);
    log_debug1("Dispatcher end");
    log_warning("End Incubus %u.%u.%u (%s)",
                VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, __TIMESTAMP__);
    return 0;
}
