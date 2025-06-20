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
    Asserts that the given token as the same string, type, and length
 */
#define ASSERT_TOKEN_EQUALS(token, string, kw_type, length) { \
    ASSERT(strcmp(token.str, string) == 0);                   \
    ASSERT(token.type == kw_type);                            \
    ASSERT(token.len == length);                              \
}

/*
    Tests that invalid inputs are handled correctly 
 */
void test_invalid(int *failing, int *run) {
    (*run)++;
    
    printf("Beginning token_list test with invalid inputs. Ignore the following error messages.\n");
    // Test with improperly formatted quoted strings
    token_list *result = tokenize("this quote is \"not closed");
    ASSERT(result == NULL);

    result = tokenize("So is \"this on\"e");
    ASSERT(result == NULL);

    printf("Finished invalid input testing for token_list\n");
}

/*
    Tests that keywords are found correctly
 */
void test_keywords(int *failing, int *run) {
    
    (*run)++;

    // Make sure keywords are identified correctly
    token_list *result = tokenize("followed by repeated \t\t \n and or more times from to optional"
                      " \t\t\r\r\r\n\t     \r\t  character\n\n\n "
                      "between and not the set newline tab any \"quoted string\"    \"and\" a 2 \n");
    
    // Make sure all tokens are created as expected
    ASSERT(result->len == 23);
    ASSERT(result->capacity == 40);
    ASSERT_TOKEN_EQUALS(result->tokens[ 0], "followed",          KEYWORD_FOLLOWED,  8);
    ASSERT_TOKEN_EQUALS(result->tokens[ 1], "by",                KEYWORD_BY,        2);
    ASSERT_TOKEN_EQUALS(result->tokens[ 2], "repeated",          KEYWORD_REPEATED,  8);
    ASSERT_TOKEN_EQUALS(result->tokens[ 3], "and",               KEYWORD_AND,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[ 4], "or",                KEYWORD_OR,        2);
    ASSERT_TOKEN_EQUALS(result->tokens[ 5], "more",              KEYWORD_MORE,      4);
    ASSERT_TOKEN_EQUALS(result->tokens[ 6], "times",             KEYWORD_TIMES,     5);
    ASSERT_TOKEN_EQUALS(result->tokens[ 7], "from",              KEYWORD_FROM,      4);
    ASSERT_TOKEN_EQUALS(result->tokens[ 8], "to",                KEYWORD_TO,        2);
    ASSERT_TOKEN_EQUALS(result->tokens[ 9], "optional",          KEYWORD_OPTIONAL,  8);
    ASSERT_TOKEN_EQUALS(result->tokens[10], "character",         KEYWORD_CHARACTER, 9);
    ASSERT_TOKEN_EQUALS(result->tokens[11], "between",           KEYWORD_BETWEEN,   7);
    ASSERT_TOKEN_EQUALS(result->tokens[12], "and",               KEYWORD_AND,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[13], "not",               KEYWORD_NOT,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[14], "the",               KEYWORD_THE,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[15], "set",               KEYWORD_SET,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[16], "newline",           KEYWORD_NEWLINE,   7);
    ASSERT_TOKEN_EQUALS(result->tokens[17], "tab",               KEYWORD_TAB,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[18], "any",               KEYWORD_ANY,       3);
    ASSERT_TOKEN_EQUALS(result->tokens[19], "quoted string",     LITERAL_STRING,   13);
    ASSERT_TOKEN_EQUALS(result->tokens[20], "and",               LITERAL_STRING,    3);
    ASSERT_TOKEN_EQUALS(result->tokens[21], "a",                 IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[22], "2",                 UNKNOWN_TOKEN,     1);

    destroy_token_list(result);
}

/*
    Tests for the tokenize() function
 */
void test_tokenize(int *failing, int *run) {
    
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
    ASSERT_TOKEN_EQUALS(result->tokens[ 0], "a",   IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 1], "b",   IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 2], "c",   IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 3], "[",   SPECIAL_CHARACTER, 1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 4], "def", IDENTIFIER   ,     3);
    ASSERT_TOKEN_EQUALS(result->tokens[ 5], "]",   SPECIAL_CHARACTER, 1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 6], "(",   SPECIAL_CHARACTER, 1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 7], "gh",  IDENTIFIER,        2);
    ASSERT_TOKEN_EQUALS(result->tokens[ 8], ")",   SPECIAL_CHARACTER, 1);
    ASSERT_TOKEN_EQUALS(result->tokens[ 9], ";",   SPECIAL_CHARACTER, 1);
    ASSERT_TOKEN_EQUALS(result->tokens[10], "i",   IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[11], "j",   IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[12], "k",   IDENTIFIER,        1);
    ASSERT_TOKEN_EQUALS(result->tokens[13], ":",   SPECIAL_CHARACTER, 1);

    destroy_token_list(result);
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
    
    test_tokenize(&local_failed, &local_run);
    test_keywords(&local_failed, &local_run);
    test_invalid(&local_failed, &local_run);
    
    // Print total pass and fail amounts, then update the global pass and fail amounts
    printf("FAILING TOKEN LIST TESTS: %d\nTOKEN LIST TESTS RUN: %d\n", local_failed, local_run);
    *failing += local_failed;
    *run += local_run;
}
