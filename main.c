#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_list.h"

/*
    The entry point of the program. Right now, it only runs a short test
 */
int main(int argc, char **argv) {
    
    char *test_str = "This is a \"test of\" my \"scanner that \\\"I wrote\\\" for this project\" test test test this is a long test to make sure that the doubling of capacity works as expected";

    token_list *list = tokenize(test_str);
    
    print_token_list(list); 

    destroy_token_list(list);

    return 0;
}
