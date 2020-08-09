#ifndef __LIBTEST_H__
#define __LIBTEST_H__

#include <stdbool.h>

void libtest_start();
void libtest_startTestCase(const char *name);
void libtest_endTestCase();
void libtest_assert(bool expression, const char *message, const char *testCaseFile, const char *testCaseName, int assertionLine);
void libtest_finish();

#define BEGIN_TEST_CASE libtest_startTestCase(__func__)
#define END_TEST_CASE libtest_endTestCase()
#define ASSERT(expression, message) libtest_assert(expression, message, __FILE__, __func__, __LINE__)

#endif
