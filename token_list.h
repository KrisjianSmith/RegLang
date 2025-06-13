/*
    token_list.h
    Krisjian Smith
    6/11/2025

    The token list stores the input file to RegLang as a list of tokens, with whitespace
    ignored. This is the first step taken in the compilation process.
    
    Whitespace will be ignored, except where it is contained within non-escaped quotations.
    For example, the string:

    The quick brown "fox jumped over \"the lazy\" dog"

    Would be broken into the following list:

        The
        quick
        brown
        "fox jumped over \"the lazy\" dog"

    Quoted strings, as shown in the example above, must have a closing quote that is followed
    by whitespace, or the quotation mark must be the end of the string. For example, the following
    string is invalid:
        
        this is "a string"abc
 */

#ifndef TOKEN_LIST
#define TOKEN_LIST

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
    The type of keyword that a token is. This will be used later
    in the compilation process to prevent having to do costly
    string comparisons
 */
typedef enum { 
    NOT_A_KEYWORD,
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
} keyword_type;

// The number of keywords in the above enumeration
#define NUMBER_OF_KEYWORDS 18

/*
    Struct to represent a token
 */
typedef struct {
    
    // The string representation of this token
    char *str;
    
    // The keyword, or NOT_A_KEYWORD if not a keyword
    keyword_type type;
    
    // The length of the string representation, excluding null terminators
    int len;

} token;


/*
    Struct that represents a list of tokens.
 */
typedef struct {
    
    // The number of tokens in the list
    int len;

    // The amount of tokens in the list
    int capacity;

    // The list of tokens itself
    token *tokens;
} token_list;

/*
    Takes a string as input and turns it into a token list as described above. If
    the input is invalid, the program will print an error message and exit.

    Inputs:
        str - the string to tokenize. This must be a null-terminated string.

    Outputs:
        Returns a token list containing all tokens in the given string. 
 */
token_list *tokenize(const char *str);

/*
    Destroys the given token list, freeing all of its reserved memory in the
    process.

    Inputs:
        list - the token list to destroy
 */
void destroy_token_list(token_list* list);

/*
    Prints the given list in a readable format.
    Useful for debugging.
    
    Inputs:
        list - the list to print
 */
void print_token_list(token_list *list);

#endif
