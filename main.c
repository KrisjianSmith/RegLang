#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_list.h"

/*
    The entry point of the program. Right now, it only runs a short test
 */
int main(int argc, char **argv) {
    
    // Check for proper arguments
    if (argc != 2) {
        printf("Usage: RegLang [string to tokenize]\n");
        return 1;
    }

    printf("Tokenizing string:\n    %s\n", argv[1]);

    // Try to tokenize the first input
    token_list *list = tokenize(argv[1]);
    
    print_token_list(list); 

    destroy_token_list(list);

    return 0;
}
