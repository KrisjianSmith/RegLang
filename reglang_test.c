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
    int run_tests = 0;
    printf("STARTING REGLANG TESTS\n");
    
    // Run tests for token_list.h
    run_token_list_tests(&failed_tests, &run_tests);
    
    // Output total passed and failed tests
    printf("\n--------------------\nTOTAL FAILED TESTS: %d\nTOTAL TESTS RUN: %d\n",
        failed_tests, run_tests);

    return 0;
}
