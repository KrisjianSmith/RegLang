#include "parse_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
    parse_tree.h
    Krisjian Smith
    6/13/2025

    The parse tree is the representation of a RegLang expression built by interpreting
    the token list using a context free grammer.
 */

/*
    Returns a parse tree node representing the file if the given token list represents
    a valid RegLang file, or NULL otherwise
 */
parse_tree_node *get_file(token_list *list, int *starting_index) {
    return NULL;
}

/*
    Returns a parse tree node representing the next expression
    in the token list, if one exists, or NULL otherwise
 */
parse_tree_node *get_assignment(token_list *list, int *starting_index) {
    return NULL;
}
/*
    Returns true if the given token list represents a valid expression
 */
parse_tree_node *get_expression(token_list *list, int *starting_index) {
    return false;
}
/*
    Returns true if the given token list represents a valid expr_start
 */
parse_tree_node *get_expr_start(token_list *list, int *starting_index) {
    return false;
}

/*
    Creates a parse tree from the given token list. The caller must eventually
    destroy the parse tree with the destroy_parse_tree function to free its memory.
 */
parse_tree *create_parse_tree(token_list *list) {
    
    return NULL;
}

/*
    Destroys the given parse tree, freeing all memory in the process. Note that this
    does NOT destroy the memory of the token list usd to create it, that must be
    destroyed with the destroy_token_list method defined in the token_list.h file.
 */
void destroy_parse_tree(parse_tree *tree) {
    // TODO: this
}

/*
    Turns the given parse tree into its regex representation
 */
char *to_regex(parse_tree *tree) {
    return NULL;
}
