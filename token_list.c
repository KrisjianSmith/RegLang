#include "token_list.h"
#include <stdbool.h>
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
    ESCAPE,
    AFTER_SINGLE_CHAR_TOKEN
} fsm_state;

/*
    Returns true if the given character is whitespace
 */
bool is_whitespace(char c) {
    switch (c) {
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            return true;
            break;
        default:
            return false; 
    }
}

/*
    Returns true if the given character represents a single character token.
    These tokens need to be handled separately, as they are allowed to exist
    without surrounding whitespace, unlike other tokens
 */
bool is_single_char_token(char c) {
    switch (c) {
        case '[':
        case ']':
        case '(':
        case ')':
        case ':':
        case ';':
            return true;
            break;
        default:
            return false;
    }
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
    Returns the enumeration for the type of keyword provided.
    If the given string is not a keyword, then NOT_A_KEYWORD will
    be returned

    str - The string to get the enumeration of
    Returns the keyword_type enumeration of the given keyword
 */
keyword_type get_keyword_type(const char *str) {
    
    // List of all keywords
    char keywords[][16] = {
        "followed",
        "by",
        "repeated",
        "or",
        "more",
        "times",
        "from",
        "to",
        "optional",
        "character",
        "between",
        "and",
        "not",
        "the",
        "set",
        "newline",
        "tab",
        "any" 
    };
    
    // List of all corresponding keyword enumeration values
    keyword_type types[] = {
        KEYWORD_FOLLOWED,
        KEYWORD_BY,
        KEYWORD_REPEATED,
        KEYWORD_OR,
        KEYWORD_MORE,
        KEYWORD_TIMES,
        KEYWORD_FROM,
        KEYWORD_TO,
        KEYWORD_OPTIONAL,
        KEYWORD_CHARACTER,
        KEYWORD_BETWEEN,
        KEYWORD_AND,
        KEYWORD_NOT,
        KEYWORD_THE,
        KEYWORD_SET,
        KEYWORD_NEWLINE,
        KEYWORD_TAB,
        KEYWORD_ANY
    };
    
    // Iterate through the keywords. If a match is found, return
    // that value
    for (int i = 0; i < NUMBER_OF_KEYWORDS; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return types[i];
        }
    }
    
    // If no match is found, return NOT_A_KEYWORD 
    return NOT_A_KEYWORD;
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
        list->tokens = (token *)realloc(list->tokens, sizeof(token) * list->capacity);
    }
    
    // Get the reference to the next token
    token *t = list->tokens + list->len;
    
    // Allocate space for the token's string and copy the contents
    t->str = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(t->str, str, len);
    t->str[len] = '\0';
    t->len = len;
    t->type = get_keyword_type(str);

    // Update the length of the token list
    list->len++;
}

/*
    Destroys a token list. Frees the memory of the list and
    all tokens it contains
 */
void destroy_token_list(token_list* list) {
    
    // Free the tokens
    for (int i = 0; i < list->len; i++) {
        free(list->tokens[i].str);
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
    list->tokens = (token *)malloc(sizeof(token) * list->capacity);

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

                // If we find a single character token, record it and move on
                if (is_single_char_token(str[index])) {
                    add_string(list, &str[index], 1);
                    state = AFTER_SINGLE_CHAR_TOKEN;
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
                
                // If we found a single character token, record the previous token and
                // the single character token in the list
                if (is_single_char_token(str[index])) {

                    // Add the token to the list
                    int len = index - starting_index;
                    add_string(list, &str[starting_index], len);

                    // Add the single character token to the list
                    add_string(list, &str[index], 1);

                    // Set the state and move on
                    state = AFTER_SINGLE_CHAR_TOKEN;
                    break;
                }

                // If we see any non-whitespace character, keep reading the token
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

                // Ignore this character and move back to the reading string state
                state = READING_STRING;   
            break;

            case END_OF_STRING:
                
                // If a string just ended, there must be whitespace or a single character
                // token after it. Otherwise, the input string is invalid.
                if (is_whitespace(str[index])) {
                    
                    // Add the string to the list of tokens
                    int len = index - starting_index;
                    add_string(list, &str[starting_index], len);
                    state = SKIPPING_WHITESPACE;
                }
                else if (is_single_char_token(str[index])) {
                     
                    // Add the string to the list of tokens
                    int len = index - starting_index;
                    add_string(list, &str[starting_index], len);
                    
                    // Add the single character token
                    add_string(list, &str[index], 1);
                    state = AFTER_SINGLE_CHAR_TOKEN;
                }
                else {
                    destroy_token_list(list);
                    fail("Improperly formatted input. Quoted string ended with no whitespace after it.");
                }
            break;

            case AFTER_SINGLE_CHAR_TOKEN:
                
                // If there is whitespace, ignore it
                if (is_whitespace(str[index])) {
                    state = SKIPPING_WHITESPACE;
                    break;
                }

                // If there is a quote, start reading the string
                if (str[index] == '\"') {
                    starting_index = index;
                    state = READING_STRING;
                    break;
                }

                // If there is another single character token, record it
                if (is_single_char_token(str[index])) {
                    add_string(list, &str[index], 1);
                    state = AFTER_SINGLE_CHAR_TOKEN;
                    break;
                }

                // Any other character means we need to start reading the next token
                starting_index = index;
                state = READING_TOKEN;
            break;
            
            // This should never be reached. If it is, something went seriously wrong.
            default:
                fail("Unexpected state in tokenizer FSM");
            break;

        }

        index++;
    }

    // After the input has been read, we may still have one leftover token
    // that hasn't been processed yet.

    // If we were reading whitespace or had just read a single character token,
    // then this token doesn't exist and we can return immediately
    if (state == SKIPPING_WHITESPACE || state == AFTER_SINGLE_CHAR_TOKEN) {
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
        token token = list->tokens[i];
        printf("    len: %d, type: %d, str: [%s]\n", token.len, token.type, token.str);
    }
}
