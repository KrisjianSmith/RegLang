/*
    Simple unit test program for RegLang
    Krisjian Smith
    6/16/2025
 */

#include <stdio.h>
#include "token_list_test.h"

/*
    Start test program. Runs all tests, and outputs a statement to
    the amount of passed/failed tests
 */
int main() {
    
    // Run all tests
    int failed_tests = 0;
    int passed_tests = 0;
    printf("STARTING REGLANG TESTS\n");
    
    // Run tests for token_list.h
    run_token_list_tests(&failed_tests, &passed_tests);
    
    // Output total passed and failed tests
    printf("\n--------------------\nTOTAL FAILED TESTS: %d\nTOTAL PASSED TESTS: %d\n",
        failed_tests, passed_tests);

    return 0;
}
