#ifndef CTESTCASE_H
#define CTESTCASE_H

#include <random>
#include <ctime>

#include "test_common.h"

/*!
 * \brief sTestPerformanceAspect - аспект производительности теста
 */
struct sTestPerformanceAspect
{
    /*!
     * \brief Start - анализ производительности начат
     */
    void Start() {tm_start = std::chrono::high_resolution_clock::now();}
    /*!
     * \brief End - анализ производительности закончен
     */
    void End()   {tm_end = std::chrono::high_resolution_clock::now();}
    /*!
     * \brief Inc - выполнено очередное действие
     */
    void Inc()   {value++;}
    /*!
     * \brief Print - получить результаты производительности в виде строки
     * \return строка с результатами производительности
     */
    std::string Print() const
    {
        char s[128];
        uint_64 elapsed = static_cast<uint_64>((std::chrono::duration_cast<std::chrono::milliseconds>(tm_end - tm_start)).count());
        snprintf(s, sizeof(s), "%s : total %lu; speed %ld per sec;",
                 name.c_str(), value, (elapsed != 0) ? (value*1000)/elapsed : 0);
        return s;
    }

    std::string	    name;           ///< название аспекта
    uint_64          value = 0;      ///< количество выполненных операций

    TimePoint_t	    tm_start;       ///< время начала замера производительности
    TimePoint_t	    tm_end;         ///< время окончания замера производительности
};

/*!
 * \brief sTestPerformance - структура производительности теста
 */
struct sTestPerformance
{
    std::string							test_name;      ///< название теста
    std::vector<sTestPerformanceAspect>	perfomance;     ///< аспекты производительности
};

/*!
 * \brief CTestCase - класс отдельного теста
 */
class CTestCase
{
public:
    CTestCase();
    virtual ~CTestCase() = default;
    /*!
     * \brief Test - запуск теста
     * \param[out] test_perf - сюда будет записана информация о производительности
     * \return успех тестирования
     */
    virtual bool Test(sTestPerformance &test_perf) = 0;
protected:
    /*!
     * \brief Random32 - сгенерировать случайное 32-разрядное число
     * \return случайное 32-разрядное число
     */
    uint_32 Random32();
    /*!
     * \brief Random32 - сгенерировать случайное 64-разрядное число
     * \return случайное 64-разрядное число
     */
    uint_64 Random64();
private:
    std::random_device  m_rd;
    std::mt19937*       m_rnd_gen;
};

using CTestCasePtr = std::shared_ptr<CTestCase>;

#endif // CTESTCASE_H
