#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool libtest_runningTestCase = false;
int libtest_testCaseCount = 0;
int libtest_assertionCount = 0;
int libtest_failedAssertionCount = 0;
int libtest_failedCaseCount = 0;
const char *libtest_currentTestCaseName = NULL;
bool libtest_currentCaseFailed = false;

void libtest_start() {
    libtest_runningTestCase = false;
    libtest_testCaseCount = 0;
    libtest_assertionCount = 0;
    libtest_failedAssertionCount = 0;
    libtest_failedCaseCount = 0;
    libtest_currentTestCaseName = NULL;
    libtest_currentCaseFailed = false;
}

void libtest_startTestCase(const char *name) {
    if(libtest_runningTestCase) {
        fprintf(stderr, "libtest_startTestCase(): Tried to start a test case while another was not previously ended.\n");
        exit(0);
    }

    libtest_runningTestCase = true;
    libtest_testCaseCount++;
    libtest_currentTestCaseName = name;
    libtest_currentCaseFailed = false;
}

void libtest_endTestCase() {
    if(!libtest_runningTestCase) {
        fprintf(stderr, "libtest_endTestCase(): Tried to end a test case while no test case was previously started.\n");
        exit(0);
    }

    libtest_runningTestCase = false;
    libtest_currentTestCaseName = NULL;
}

void libtest_assert(bool expression, const char *message, const char *testCaseFile, const char *testCaseName, int assertionLine) {
    libtest_assertionCount++;

    if(!(expression)) {\
        fprintf(stderr, "libtest_assert(): An assertion failed.\n");
        fprintf(stderr, "Test case: %s\n", testCaseName);
        fprintf(stderr, "Test file: %s\n", testCaseFile);
        fprintf(stderr, "File line: %d\n", assertionLine);
        fprintf(stderr, "Message  : %s\n", message);
        libtest_failedAssertionCount++;

        if(!libtest_currentCaseFailed) {
            libtest_currentCaseFailed = true;
            libtest_failedCaseCount++;
        }
    }
}

void libtest_finish() {
    printf("libtest_finish(): Test results:\n");
    printf("Test case count     : %d\n", libtest_testCaseCount);
    printf("Assertion count     : %d\n", libtest_assertionCount);
    printf("Assertion fail count: %d\n", libtest_failedAssertionCount);
    printf("Case fail count     : %d\n", libtest_failedCaseCount);

    if(libtest_runningTestCase) {
        printf("libtest_finish(): Warning! A test case (%s) did not end correctly and is missing END_TEST_CASE statement.\n", libtest_currentTestCaseName);
    }
}
