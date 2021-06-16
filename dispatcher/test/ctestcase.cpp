#include "ctestcase.h"

CTestCase::CTestCase()
{
    m_rnd_gen = new std::mt19937(m_rd());
}

uint_32 CTestCase::Random32()
{
    std::uniform_int_distribution<uint_32> uid(0, ~static_cast<uint_32>(0));
    return uid(*m_rnd_gen);
}

uint_64 CTestCase::Random64()
{
    std::uniform_int_distribution<uint_64> uid(0, ~static_cast<uint_64>(0));
    return uid(*m_rnd_gen);
}
