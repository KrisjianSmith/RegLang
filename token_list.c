#include "token_list.h"
#define TOKEN_LIST_STARTING_CAPACITY 10

/*
    token_list.c
    Krisjian Smith
    6/11/2025

    The token list stores the input file to RegLang as a list of tokens, with whitespace
    ignored. This is the first step taken in the compilation process.
 */

/*
    Represents the state that the scanner's finite state machine is
    currently in
 */
typedef enum {
    SKIPPING_WHITESPACE,
    READING_TOKEN,
    READING_STRING,
    END_OF_STRING,
    ESCAPE
} fsm_state;

/*
    Returns true if the given character is whitespace
 */
int is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}


/*
    Exit unsucessfully and print the given message to
    standard error
 */
void fail(char *msg) {
    fprintf(stderr, "FATAL ERROR: %s\n", msg);
    exit(1);
}

/*
    Adds the given string to the given token_list. Grows the
    list's internal array if necessary.

    list - the list to add the token to
    str - a pointer to the start of the string
    len - the length of the token

    Note that the added token will be malloc'd. This will be freed when the
    token list is destroyed.
 */
void add_string(token_list *list, const char *str, int len) {
    
    // If there is no more space in the array, double it's size
    if (list->len == list->capacity) {
        list->capacity *= 2;
        list->tokens = (char **)realloc(list->tokens, sizeof(char *) * list->capacity);
    }

    // Add the token to the list
    list->tokens[list->len] = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(list->tokens[list->len], str, len);
    list->tokens[list->len][len] = '\0';
    list->len++;
}

/*
    Destroys a token list. Frees the memory of the list and
    all tokens it contains
 */
void destroy_token_list(token_list* list) {
    
    // Free the tokens
    for (int i = 0; i < list->len; i++) {
        free(list->tokens[i]);
    }

    // Free the list
    free(list->tokens);
    free(list);
}

/*
    Takes a string as input and breaks it into several smaller strings.
    Whitespace will be ignored, except where it is contained within non-escaped quotations.
    For example, the string:

    The quick brown "fox jumped over \"the lazy\" dog"

    Would be broken into the following list:

        The
        quick
        brown
        "fox jumped over \"the lazy\" dog"

    Inputs:
        str - The string to scan

    Outputs:
        Returns a token_list struct with the tokens of the input string

    Note that the caller is responsible for freeing the returned token_list
    with the destroy_token_list() function.
 */
token_list *tokenize(const char *str) {
    
    // Allocate space for token_list. Initialize it with an empty
    // list that could hold ten tokens
    token_list *list = (token_list *)malloc(sizeof(token_list));
    list->capacity = 10;
    list->len = 0;
    list->tokens = (char **)malloc(sizeof(char *) * list->capacity);

    // Start from the beginning of the string and go to the end
    int index = 0;

    // This scanner is implemented with a finite state machine. This integer determines the
    // records the state that the machine is in
    fsm_state state = SKIPPING_WHITESPACE;

    // The first index of the next token
    int starting_index = 0;

    // Scan through the list
    while (str[index] != '\0') {
        
        switch(state) {
            
            case SKIPPING_WHITESPACE:
                
                // If we are continuing to read whitespace, do nothing
                if (is_whitespace(str[index])) {
                    break;
                }

                // If the next character is not whitespace, record this as the
                // starting index
                starting_index = index;

                // If we see a quotation mark, start reading the string
                if (str[index] == '\"') {
                    state = READING_STRING;
                }
                // If we see anything else, start reading the token
                else {
                    state = READING_TOKEN;
                }

            break;


            case READING_TOKEN:
                
                // If we see any non-whitespace character, keep reading the string
                if (!is_whitespace(str[index])) {
                    break; 
                } 
                // If we see whitespace, record the token in the token list
                else {
                    
                    // Add the token to the list
                    int len = index - starting_index;
                    add_string(list, &str[starting_index], len);

                    // Change state
                    state = SKIPPING_WHITESPACE;

                }

            break;
            

            case READING_STRING:
                
                // If we see a slash, switch to the escape state
                if (str[index] == '\\') {
                    state = ESCAPE;
                }

                // If we see a closing quotation, add the string to the
                // token list, then move to the end of quote state
                else if (str[index] == '\"') {
                    state = END_OF_STRING;
                }

                // Otherwise, stay in this state, even if there is whitespace
                else {
                    break;
                }

            break;

            case ESCAPE:

                // If there is another slash, stay in this state
                if (str[index] == '\\') {
                    break;
                }

                // Otherwise, move back to the reading string state
                else {
                    state = READING_STRING;   
                }
            break;

            case END_OF_STRING:
                
                // If a string just ended, there must be whitespace after it. Otherwise,
                // the input string is invalid.
                if (is_whitespace(str[index])) {
                    
                    // Add the string to the list of tokens
                    int len = index - starting_index;
                    add_string(list, &str[starting_index], len);
                    state = SKIPPING_WHITESPACE;
                }
                else {
                    destroy_token_list(list);
                    fail("Improperly formatted input. Quoted string ended with no whitespace after it.");
                }
            break;

        }

        index++;
    }

    // After the input has been read, we may still have one leftover token
    // that hasn't been processed yet.

    // If we were reading whitespace, then this token doesn't exist and we can
    // return immediately
    if (state == SKIPPING_WHITESPACE) {
        return list;
    }

    // If we were reading a token or had just finished reading a quoted string, then
    // this token or quoted string needs to be added to the list. We can then return.
    if (state == READING_TOKEN || state == END_OF_STRING) {
        int len = index - starting_index;
        add_string(list, &str[starting_index], len);
        return list;
    }

    // Otherwise, we stopped while reading a quoted string with no closing quote. Therefore, the
    // input is invalid
    destroy_token_list(list);
    fail("Improperly formatted input. Quoted string has no closing quote.");
    return NULL;
}

/*
    Prints the given list in a readable format. Used for debugging.
    list - the list to print
 */
void print_token_list(token_list *list) {
    
    printf("Length: %d\nCapacity: %d\nTokens:\n", list->len, list->capacity);

    for (int i = 0; i < list->len; i++) {
        printf("    [%s]\n", list->tokens[i]);
    }
}
