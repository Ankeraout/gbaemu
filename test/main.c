#include <stdio.h>
#include <stdlib.h>

#include "libtest.h"
#include "test_dummy.h"

int main(int argc, const char **argv) {
    libtest_start();

    test_dummy();
    
    libtest_finish();

    return EXIT_SUCCESS;
}
