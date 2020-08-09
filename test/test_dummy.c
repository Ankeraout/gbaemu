#include <stdbool.h>

#include "libtest.h"

/* Description: Example test case.
 * Returns: 0 if the test passed, 1 if the test failed.
 */
void test_dummy() {
    BEGIN_TEST_CASE;
    ASSERT(true, "true is false.");
    END_TEST_CASE;
}
