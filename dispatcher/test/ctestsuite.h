#ifndef CTESTSUITE_H
#define CTESTSUITE_H

#include "ctestcase.h"

/*!
 * \brief CTestSuite - набор тестов
 */
class CTestSuite
{
public:
	CTestSuite() = default;
    /*!
     * \brief ProcArgument - обработать аргумент командной строки и запустить соответствующий ему тест
     * \param[in] arg - аргумент командной строки
     * \return успех теста
     */
    bool ProcArgument(const char* arg);
};

#endif // CTESTSUITE_H
