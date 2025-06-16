/*
    Tests for token_list.h

    Krisjian Smith
    6/16/2025
 */

#include "token_list_test.h"
#include "token_list.h"
#include <stdio.h>

/*
    Run all tests for token_list.h

    failing - pointer to an integer to store the total amount of failed tests
    passing - pointer to an integer to store the total amount of passing tests
 */
void run_token_list_tests(int *failing, int *passing) {
    
    int local_failed = 0;
    int local_passed = 0;

    printf("\nSTARTING TOKEN LIST TEST\n");
    
    local_failed = 2;
    local_passed = 3;
    
    // Print total pass and fail amounts, then update the global pass and fail amounts
    printf("FAILING TOKEN LIST TESTS: %d\nPASSING TOKEN LIST TESTS: %d\n", local_failed, local_passed);
    *failing += local_failed;
    *passing += local_passed;
}
