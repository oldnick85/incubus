#include "unittest.h"

CUnitTest::CUnitTest(ILoggerPtr log)
    : CLogged(log)
{

}

bool CUnitTest::TestCollisionMovingPoint()
{
    log_warning("TestCollisionMovingPoint");
    uint errors_count = 0;

    bool test1_OK = true;
    CShape shape_circ;
    shape_circ.AddCircle({0.0, 0.0}, 1.0);
    sCollision collision;
    shape_circ.CollisionMovingPoint({0.0, 0.0}, {-1.5, -0.1}, {1.0, 0.0}, &collision);
    if (not collision.collided) test1_OK = false;
    if (collision.point.x > 0.0) test1_OK = false;
    if (collision.direction.vx < 0.0) test1_OK = false;
    if (not test1_OK)
        errors_count++;
    log_warning("test #1 ... %s", test1_OK ? "OK" : "ERROR");

    log_warning("errors=%u", errors_count);
    return (errors_count == 0);
}
