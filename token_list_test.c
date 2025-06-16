/*
    Tests for token_list.h

    Krisjian Smith
    6/16/2025
 */

#include "token_list_test.h"
#include "token_list.h"
#include <stdio.h>

// TODO: Move this to its own header file to be reused in other tests
/*
    Simple assert macro. If the given expression is false, a message is printed to the console
    stating that the test failed, and the failing test counter is incremented. The function will
    return, meaning that nothing else in that function will be tested.

    This will cause memory leaks if any results are not freed before being asserted, but this is fine for
    testing purposes
 */
#define ASSERT(expr) if (!(expr)) { printf("Failed test: %s: %d\n", __FILE__, __LINE__); (*failing)++; return; }

/*
    Tests for the tokenize() function
 */
void test_token_list(int *failing, int *run) {
    
    (*run)++;

    // An empty string should give an empty token list
    token_list *result = tokenize("");

    ASSERT(result->len == 0);
    ASSERT(result->capacity == 10);
    ASSERT(result->tokens != NULL);

    destroy_token_list(result); 

    // Make sure whitespace is ignored properly
    result = tokenize("a b c [def](gh);\ni\tj\rk:");
    
    // Make sure all tokens are created as expected
    ASSERT(result->len == 14);
    ASSERT(result->capacity == 20);
}    


/*
    Run all tests for token_list.h

    failing - pointer to an integer to store the total amount of failed tests
    run - pointer to an integer to store the total amount of tests run
 */
void run_token_list_tests(int *failing, int *run) {
    
    int local_failed = 0;
    int local_run = 0;

    printf("\nSTARTING TOKEN LIST TEST\n");
    
    test_token_list(&local_failed, &local_run);
    
    // Print total pass and fail amounts, then update the global pass and fail amounts
    printf("FAILING TOKEN LIST TESTS: %d\nTOKEN LIST TESTS RUN: %d\n", local_failed, local_run);
    *failing += local_failed;
    *run += local_run;
}
