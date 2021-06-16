#ifndef UNITTEST_H
#define UNITTEST_H
#include "incubus_common.h"
#include "shape.h"
#include "i_logger.h"

class CUnitTest
        : public CLogged
{
public:
    CUnitTest(ILoggerPtr log);

    bool TestCollisionMovingPoint();

private:
};

#endif // UNITTEST_H
