#ifndef PARSE_TREE
#define PARSE_TREE

#include "token_list.h"

/*
    parse_tree.h
    Krisjian Smith
    6/13/2025

    The parse tree is the representation of a RegLang expression built by interpreting
    the token list using a context free grammer.
 */


/*
    Node in the parse tree representation of the RegLang expression
 */
typedef struct ptn {
    
    // The list of child nodes
    struct ptn *children;
    
    // The number of child nodes
    int len;
    
    // The regex representation of this node, or
    // NULL if it cannot be created yet
    char* regex;

} parse_tree_node;

/*
    Parse tree representation of the RegLang expression
 */
typedef struct {
    
    // The root of the tree is special, in that it can have any number of
    // children. Each assignment in the file will have its own node.
    parse_tree_node *assignments;
    
    // How many assignments are in the file
    int assignments_len;
    
    // How many assignments this node can have before it needs to be
    // resized
    int capacity;

} parse_tree;

/*
    Creates a parse tree from the given token list. The caller must eventually
    destroy the parse tree with the destroy_parse_tree function to free its memory.
 */
parse_tree *create_parse_tree(token_list *list);

/*
    Destroys the given parse tree, freeing all memory in the process. Note that this
    does NOT destroy the memory of the token list usd to create it, that must be
    destroyed with the destroy_token_list method defined in the token_list.h file.
 */
void destroy_parse_tree(parse_tree *tree);

/*
    Turns the given parse tree into its regex representation
 */
char *to_regex(parse_tree *tree);

#endif
